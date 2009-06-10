#include <QTimer>
#include <QFileInfo>

#include <qendian.h>
#include <qtopianamespace.h>
#include <qtopialog.h>

#include "mp3decoder.h"

#include <errno.h>
#include <stdlib.h>


// {{{ Mp3DecoderPrivate
class Mp3DecoderPrivate
{
public:
    bool                initialized;
    bool                muted;
    int                 volume;
    quint32             length;
    quint32             position;
    QMediaDevice*       inputDevice;
    QtopiaMedia::State  state;
    QMediaDevice::Info  outputInfo;

    mad_fixed_t       attenuate;
    struct filter     *filters;

    struct mad_stream stream;
    struct mad_frame  frame;
    struct mad_synth  synth;

    mad_fixed_t       buffer[2][65536 * 2];
};
// }}}

// {{{ Mp3Decoder
Mp3Decoder::Mp3Decoder():
    d(new Mp3DecoderPrivate)
{
    // init
    d->initialized = false;
    d->muted = false;
    d->volume = 50;
    d->length = 0;
    d->position = 0;
    d->state = QtopiaMedia::Stopped;
    d->outputInfo.type = QMediaDevice::Info::PCM;

    input_data = (unsigned char *)malloc(MAD_BUFFER_SIZE*8);
    output_data = (unsigned char *)malloc(MAD_BUFFER_SIZE*8);
    output_pos = output_data;
    buffered=0;
    offset=0;
    input_length=0;
    output_length=0;
    resync=false;
    header=false;
    duration=0;
    init=false;
    update_duration=0;
    current=0;
    duration=0;
}

Mp3Decoder::~Mp3Decoder()
{
    mad_synth_finish(&d->synth);
    mad_frame_finish(&d->frame);
    mad_stream_finish(&d->stream);

    delete input_data;
    delete output_data;

    delete d;
}

QMediaDevice::Info const& Mp3Decoder::dataType() const
{
    return d->outputInfo;
}

bool Mp3Decoder::connectToInput(QMediaDevice* input)
{
    if (input->dataType().type != QMediaDevice::Info::Raw) {
        return false;
    }

    d->inputDevice = input;

    return true;
}

void Mp3Decoder::disconnectFromInput(QMediaDevice* input)
{
    Q_UNUSED(input);
    d->inputDevice = 0;
}

void Mp3Decoder::start()
{
    if (!d->initialized)
    {
        if (QIODevice::open(QIODevice::ReadWrite | QIODevice::Unbuffered))
        {
            mad_stream_init(&d->stream);
            mad_frame_init(&d->frame);
            mad_synth_init(&d->synth);

            fill_input_buffer();
            decode_data();

            d->outputInfo.bitsPerSample = 16;
            d->outputInfo.volume = d->volume;

            d->initialized = true;
        }
    }

    if (d->initialized)
    {
        d->state = QtopiaMedia::Playing;

        emit readyRead();
        emit playerStateChanged(d->state = QtopiaMedia::Playing);
    }
}

void Mp3Decoder::stop()
{
    emit playerStateChanged(d->state = QtopiaMedia::Stopped);
    seek(0);
}

void Mp3Decoder::pause()
{
    emit playerStateChanged(d->state = QtopiaMedia::Paused);
}

quint64 Mp3Decoder::length()
{
    return d->length;
}

bool Mp3Decoder::seek(qint64 ms)
{
    if(frames > 0) {
        qint64 rawPos = (frames*ms/d->length)*framesize;
        d->inputDevice->seek(rawPos);
    } else if (d->inputDevice->dataType().dataSize > 0) {
        qint64 rawPos = (d->inputDevice->dataType().dataSize/d->length)*ms;
        d->inputDevice->seek(rawPos);
    } else {
        return true;
    }

    update_duration=0;
    current=ms;
    input_length=0;
    output_length=0;
    output_pos=output_data;

    emit positionChanged(d->position = ms);

    return true;
}

void Mp3Decoder::setVolume(int volume)
{
    d->volume = qMin(qMax(volume, 0), 100);

    if (!d->muted)
        d->outputInfo.volume = d->volume;

    emit volumeChanged(d->volume);
}

int Mp3Decoder::volume()
{
    return d->volume;
}

void Mp3Decoder::setMuted(bool mute)
{
    d->outputInfo.volume = mute ? 0 : d->volume;

    emit volumeMuted(d->muted = mute);
}

bool Mp3Decoder::isMuted()
{
    return d->muted;
}

//private:
qint64 Mp3Decoder::readData(char *data, qint64 maxlen)
{
    if(d->state == QtopiaMedia::Playing) {
        if((d->outputInfo.frequency!=0) && (d->outputInfo.channels!=0)) {
            if((int)update_duration >= (int)(d->outputInfo.frequency*d->outputInfo.channels)) {
                update_duration=0;
                current=current+1000; // 1 second update
                duration=d->length-current;
                d->position = current;
                emit positionChanged(d->position);
            }
        }

        //If we have enough decoded data just output it and return
        if(output_length >= (int)maxlen) {
            memcpy(data,output_pos,(int)maxlen);
            output_pos=output_pos+(int)maxlen;
            output_length=output_length-(int)maxlen;
            update_duration=update_duration+(int)maxlen/2;
            return maxlen;
        } else {
            memmove(output_data,output_pos,output_length);
            output_pos=output_data+output_length;
        }

        fill_input_buffer();
        decode_data();

        if(output_length >= (int)maxlen) {
            memcpy(data,output_pos,(int)maxlen);
            output_pos=output_pos+(int)maxlen;
            output_length=output_length-(int)maxlen;
            update_duration=update_duration+(int)maxlen/2;
            return maxlen;
        }
    }
    return 0;
}

void Mp3Decoder::fill_input_buffer()
{
    //If we have more frames in input buffer move it along
    if(input_length != 0) {
        if (d->stream.next_frame) {
            if ( d->stream.next_frame > &input_data[input_length] ) {
                //qWarning("fill_input_buffer() zero input_length no memmove");
                input_length = 0;
            } else {
                input_length = &input_data[input_length] - d->stream.next_frame;
                //qWarning("fill_input_buffer() sync to next frame input_length=%d",input_length);
            }
            memmove(input_data, d->stream.next_frame, input_length);
        }
        if ( resync ) {
            input_length = 0;
            resync = false;
        }
    }

    int len=0;
    do {
        len = d->inputDevice->read((char*)input_data+input_length, MAD_BUFFER_SIZE*2-input_length);
    } while (len == -1 && errno == EINTR);
    if (len == -1) {
        qWarning("error reading audio");
        return;
    }
    input_length=input_length+len;
    //qWarning("read in %d bytes, input_length=%d",(int)len,input_length);
}

void Mp3Decoder::decode_data()
{
    if(init==false) {
        int i;
        unsigned char *c;
        unsigned char uc,uc2,uc3,uc4;

        c=(unsigned char*)input_data;

        frames=0;

        for(i=0;i<input_length-4;i++) {
            if((*(c+i)=='X')&&(*(c+i+1)=='i')&&(*(c+i+2)=='n')
                    &&(*(c+i+3)=='g')) {
                //qWarning("VBR mp3 file, Xing header detected");
                uc  = *(c+i+8);
                uc2 = *(c+i+9);
                uc3 = *(c+i+10);
                uc4 = *(c+i+11);

                int ii=1;
                char *p = (char*)&ii;
                if(p[0]==1)
                    frames = (((long)uc<<24)|((long)uc2<<16)|((long)uc3<<8)|((long)uc4));
                else
                    frames = (((long)uc4<<24)|((long)uc3<<16)|((long)uc2<<8)|((long)uc));
                break;
            }
        }
    }

    //Load the input data into decoder
    mad_stream_buffer(&d->stream, input_data, input_length);

    static const int maxBuffered = MAD_BUFFER_SIZE;

    //if ( input_length > maxBuffered ) input_length = maxBuffered;

    //Decode data
    offset = buffered;
    while ( buffered < maxBuffered ) {
        while (mad_frame_decode(&d->frame, &d->stream) == -1) {
            if ( d->stream.error == MAD_ERROR_BUFLEN ) {
                qWarning("error MAD_ERROR_BUFLEN");
                resync=true;
                d->state = QtopiaMedia::Stopped;
                emit playerStateChanged(d->state);
                return;
            }
            if ( d->stream.error == MAD_ERROR_BADCRC ) {
                mad_frame_mute(&d->frame);
                qWarning("error decoding, bad crc");
            }
        }

        d->outputInfo.frequency = d->frame.header.samplerate;
        //d->outputInfo.channels = (d->frame.header.mode > 0 ? 2: 1);
        d->outputInfo.channels = 2;

        if(init==false) {
            if(frames == 0) {
                d->length = (d->inputDevice->dataType().dataSize/(d->frame.header.bitrate/8))*1000;
            } else {
                d->length = 1152*frames/d->outputInfo.frequency*1000;
            }
            //qWarning("length=%d, frames=%d, freq=%d, channels=%d",
            //        d->length,frames,d->outputInfo.frequency,d->outputInfo.channels);

            framesize = 144*d->frame.header.bitrate/d->outputInfo.frequency;
            duration= d->length;
            current=0;
            emit lengthChanged(d->length);
            init=true;
        }
        mad_synth_frame(&d->synth, &d->frame);
        int decodedSamples = d->synth.pcm.length;
        memcpy(&(d->buffer[0][offset]),d->synth.pcm.samples[0],
                decodedSamples * sizeof(mad_fixed_t) );
        if ( d->synth.pcm.channels == 2 ) {
            memcpy(&(d->buffer[1][offset]),d->synth.pcm.samples[1],
                    decodedSamples * sizeof(mad_fixed_t) );
        }
        offset += decodedSamples;
        buffered += decodedSamples;
    }
    //Convert mad_fixed_t to PCM data
    audio_pcm((unsigned short int*)output_pos,buffered,d->buffer[0],
            (d->synth.pcm.channels == 2) ? d->buffer[1] : 0 );

    int l=buffered*2;
    //if(d->synth.pcm.channels == 2) l=l*2;
    l=l*2;
    output_length=output_length+l;
    output_pos=output_data;

    memmove( d->buffer[0], &(d->buffer[0][buffered]),
            buffered * sizeof(mad_fixed_t) );
    if ( d->synth.pcm.channels == 2 )
        memmove( d->buffer[1], &(d->buffer[1][buffered]),
                buffered * sizeof(mad_fixed_t) );
    buffered=0;
}

qint64 Mp3Decoder::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

inline signed int Mp3Decoder::scale( mad_fixed_t sample )
{
    sample+=(1L<<(MAD_F_FRACBITS-16));
    sample=(sample>=MAD_F_ONE)?MAD_F_ONE-1:((sample<-MAD_F_ONE)?-MAD_F_ONE:sample);
    return sample>>(MAD_F_FRACBITS+1-16);
}

inline void Mp3Decoder::audio_pcm( unsigned short *data, unsigned int nsamples, mad_fixed_t *left, mad_fixed_t *right )
{
    if ( right ) {
        while (nsamples--) {
            *data++ = scale( *left++ );
            *data++ = scale( *right++ );
        }
    } else {
        while (nsamples--) {
            signed int mono = scale( *left++ );
            *data++ = mono;
            *data++ = mono;
        }
    }
}

// }}}


