#pragma once
#ifndef CHndFile_H
#define CHndFile_H

#include "CPieceCollection.h"
#include "CMachineCollection.h"
#include "CWorkCenterCollection.h"

#define LINE_LENGTH 512

enum MEASURE_UNIT {muSecond, muMinute, muHour, muDay};

//------------------------------------------------------------------------------
//	ClassName  : CHndFile
//
//  Description: Manipula un fichero con informacion referente a una instancia
//							 del Job Shop Scheduling Problem.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CHndFile {
	private:
		FILE *file;
		char *sFilename;
		char KeyWord[13][16];

	private:
		bool isKeyWord (const char *string);

	public:
		//--- Constructors and destructor ---//
		 CHndFile ( void );							
		 CHndFile ( const char *path );
		~CHndFile ( void );							

		//--- Set functions ---//
		void SetPath ( const char *path );

		//--- Get functions ---//
		const char *GetPath     ( void ) const;
		const char *GetFilename ( bool extension = true ) const;

		//--- File handler functions ---//
		int ReadPieceCount		 ( void );
		int ReadMachineCount	 ( void );
		int ReadInstanceSize	 ( int *job, int *machine );		
    int ReadMachineData		 ( CMachineCollection *m_collection );
		int ReadWorkCenterData ( CWorkCenterCollection *wc_collection );
		int ReadPieceData			 ( CPieceCollection *p_collection, const CMachineCollection &m_collection );
    int WriteData					 ( const CMachineCollection &MachineList, const CPieceCollection &Recipient );
		bool Close						 ( void );

		//--- Overload funtions ---//
		friend std::ostream& operator << (std::ostream &stream, const CHndFile &hnd);
};

#endif