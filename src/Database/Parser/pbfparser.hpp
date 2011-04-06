/*
    Biker wants to be a routing software, intended to be useful for planning bike tours.
    Copyright (C) 2011  Lena Brueder

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

#ifndef PBFPARSER_HPP
#define PBFPARSER_HPP

#include "src/DataPrimitives/DataPrimitives.hpp"
#include "src/Database/osmdatabasewriter.hpp"
#include "src/Database/Parser/pbf/osmformat.pb.h"
#include "src/Database/Parser/pbf/fileformat.pb.h"
#include <zlib.h>
// #include <bzlib.h>
#include <QHash>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QStringList>

#define NANO ( 1000.0 * 1000.0 * 1000.0 )
#define MAX_BLOCK_HEADER_SIZE ( 64 * 1024 )
#define MAX_BLOB_SIZE ( 32 * 1024 * 1024 )


/* Sometimes, you need to rebuild osmformat.pb.h and osmformat.pb.cc
   To do so, type
         protoc osmformat.proto --cpp_out=.
   from src/Database/Parser/pbf/ .
 */

class PBFParser
{
private:
    OSMDatabaseWriter& dbWriter;

    OSMNode* node;
    OSMWay* way;
    OSMEdge* edge;
    OSMRelation* relation;

    int nodeCount;
    int wayCount;
    int relationCount;
    
    
	enum Mode {
		ModeNode, ModeWay, ModeRelation, ModeDense
	};
    enum EntityType {
        EntityNone, EntityNode, EntityWay, EntityRelation
    };
	int convertNetworkByteOrder( char data[4] );
    
    
    OSMPBF::BlobHeader m_blockHeader;
    OSMPBF::Blob m_blob;

    OSMPBF::HeaderBlock m_headerBlock;
    OSMPBF::PrimitiveBlock m_primitiveBlock;

    int m_currentGroup;
    int m_currentEntity;
    bool m_loadBlock;

    Mode m_mode;

    QHash< QString, int > m_nodeTags;
    QHash< QString, int > m_wayTags;
    QHash< QString, int > m_relationTags;

    long long m_lastDenseID;
    long long m_lastDenseLatitude;
    long long m_lastDenseLongitude;
    int m_lastDenseTag;

    QFile m_file;
    QByteArray m_buffer;
    QByteArray m_bzip2Buffer;
    
    struct Tag {
        QString key;
        QString value;
    };

    struct Node {
        unsigned id;
        GPSPosition coordinate;
        std::vector< Tag > tags;
    };

    struct Way {
        unsigned id;
        std::vector< unsigned > nodes;
        std::vector< Tag > tags;
    };

    struct RelationMember {
        unsigned ref;
        enum Type {
            Way, Node, Relation
        } type;
        QString role;
    };

    struct Relation {
        unsigned id;
        std::vector< RelationMember > members;
        std::vector< Tag > tags;
    };
    
        EntityType getEntitiy( OSMNode* node, Way* way, Relation* relation );
        void parseNode( OSMNode* node );
	void parseWay( Way* way );
	void parseRelation( Relation* relation );
        void parseDense( OSMNode* node );
	void loadGroup();
	void loadBlock();
	bool readNextBlock();
	bool readBlockHeader();
	bool readBlob();
	bool unpackZlib();
	//bool unpackBzip2();
	static void *SzAlloc( void *p, size_t size);
	static void SzFree( void *p, void *address);
    
public:
    PBFParser( OSMDatabaseWriter& dbWriter );
    ~PBFParser();

    bool parse(QString filename);
};

static inline bool openQFile( QFile* file, QIODevice::OpenMode mode )
{
	if ( !file->open( mode ) ) {
		qCritical() << "could not open file:" << file->fileName() << "," << mode;
		return false;
	}
	return true;
}

#endif // PBFPARSER_HPP
