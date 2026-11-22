/*
 * Ring buffer for QtMoko stream sockets.
 *
 * Uses only public QtCore interfaces. Each reservation forms a block,
 * allowing a socket to write directly into the reserved memory without
 * moving unread bytes. Only operations needed by Bluetooth and IrDA
 * sockets are implemented.
 */
#ifndef QTOPIARINGBUFFER_P_H
#define QTOPIARINGBUFFER_P_H

#include <QByteArray>
#include <QList>
#include <QtGlobal>

class QtopiaRingBuffer
{
public:
    explicit QtopiaRingBuffer(int = 4096) : m_head(0), m_size(0) {}

    int size() const { return m_size; }
    bool isEmpty() const { return m_size == 0; }

    void clear()
    {
        m_blocks.clear();
        m_head = 0;
        m_size = 0;
    }

    char *reserve(int bytes)
    {
        Q_ASSERT(bytes > 0);
        m_blocks.append(QByteArray(bytes, '\0'));
        m_size += bytes;
        return m_blocks.last().data();
    }

    void chop(int bytes)
    {
        Q_ASSERT(bytes >= 0 && bytes <= m_size);
        while (bytes > 0) {
            QByteArray &last = m_blocks.last();
            const int available = last.size() - (m_blocks.size() == 1 ? m_head : 0);
            const int count = qMin(bytes, available);
            last.chop(count);
            m_size -= count;
            bytes -= count;
            if (last.isEmpty() || (m_blocks.size() == 1 && last.size() == m_head)) {
                m_blocks.removeLast();
                m_head = 0;
            }
        }
    }

    int nextDataBlockSize() const
    {
        return isEmpty() ? 0 : m_blocks.first().size() - m_head;
    }

    const char *readPointer() const
    {
        return isEmpty() ? 0 : m_blocks.first().constData() + m_head;
    }

    void free(int bytes)
    {
        Q_ASSERT(bytes >= 0 && bytes <= m_size);
        while (bytes > 0) {
            const int count = qMin(bytes, nextDataBlockSize());
            m_head += count;
            m_size -= count;
            bytes -= count;
            if (m_head == m_blocks.first().size()) {
                m_blocks.removeFirst();
                m_head = 0;
            }
        }
    }

    int getChar()
    {
        if (isEmpty())
            return -1;
        const unsigned char c = static_cast<unsigned char>(*readPointer());
        free(1);
        return c;
    }

    bool canReadLine() const
    {
        for (int i = 0; i < m_blocks.size(); ++i) {
            const QByteArray &block = m_blocks.at(i);
            if (block.indexOf('\n', i == 0 ? m_head : 0) != -1)
                return true;
        }
        return false;
    }

private:
    QList<QByteArray> m_blocks;
    int m_head;
    int m_size;
};

#endif // QTOPIARINGBUFFER_P_H
