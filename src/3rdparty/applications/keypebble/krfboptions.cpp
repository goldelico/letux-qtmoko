#include <QSettings>

#include <qtopiaapplication.h>
#include "krfboptions.h"

KRFBOptions::KRFBOptions()
{
  QSettings config("Trolltech","keypebble");
  config.beginGroup( "Encodings" );
  readSettings( &config );
}

KRFBOptions::~KRFBOptions()
{
    save();
}

void KRFBOptions::save()
{
  QSettings config("Trolltech","keypebble");
  config.beginGroup( "Encodings" );
  writeSettings( &config );
}

void KRFBOptions::readSettings( QSettings *config )
{
  hexTile = config->value( "HexTile" ).toInt();

  corre = config->value( "CORRE" ).toInt();
  rre = config->value( "RRE" ).toInt();
  copyrect = config->value( "CopyRect", true ).toInt();
  colors256 = config->value( "Colors256" ).toInt();
  shared = config->value( "Shared" ).toInt();
  readOnly = config->value( "ReadOnly" ).toInt();
  updateRate = config->value( "UpdateRate", 50 ).toInt();
}

void KRFBOptions::writeSettings( QSettings *config )
{
  config->setValue( "HexTile", (int)hexTile );
  config->setValue( "CORRE", (int)corre );
  config->setValue( "RRE", (int)rre );
  config->setValue( "CopyRect", (int)copyrect );
  config->setValue( "Colors256", (int)colors256 );
  config->setValue( "Shared", (int)shared );
  config->setValue( "ReadOnly", (int)readOnly );
  config->setValue( "UpdateRate", (int)updateRate );
}

int KRFBOptions::encodings()
{
  // Initially one because we always support raw encoding
  int count = 1;

  count += hexTile ? 1 : 0;
  count += corre ? 1 : 0;
  count += rre ? 1 : 0;
  count += copyrect ? 1 : 0;

  return count;
}
