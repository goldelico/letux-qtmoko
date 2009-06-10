// -*- c++ -*-

#ifndef KRFBOPTIONS_H
#define KRFBOPTIONS_H

class QSettings;

class KRFBOptions
{
public:
  KRFBOptions();
  ~KRFBOptions();

  void save();

  int encodings();
  void readSettings( QSettings *config );
  void writeSettings( QSettings *config );

  bool hexTile;
  bool corre;
  bool rre;
  bool copyrect;
    
  bool colors256;
  bool shared;
  bool readOnly;
  bool deIconify;

  int updateRate;
};

#endif // KRFBOPTIONS_H
