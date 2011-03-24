#include "osmdatabase.hpp"

OSMDatabaseReader::OSMDatabaseReader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    downloader = new SrtmDownloader("http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/Eurasia/", "./");
}
OSMDatabaseReader::OSMDatabaseReader(QString srtmUrl, QString cachedir)
{   //TODO: Sicherheitsproblem: an dieser Stelle ist es evtl. möglich, ins Dateisystem einzudringen.
    curl_global_init(CURL_GLOBAL_DEFAULT);
    downloader = new SrtmDownloader(srtmUrl, cachedir);
}
OSMDatabaseReader::~OSMDatabaseReader()
{
    curl_global_cleanup();
    delete downloader;
}

float OSMDatabaseReader::getAltitude(float lon, float lat)
{
    return downloader->getAltitudeFromLatLon(lat, lon);
}
