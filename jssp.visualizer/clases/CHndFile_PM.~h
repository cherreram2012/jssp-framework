#ifndef CHndFile_PMH
#define CHndFile_PMH

#include "CPieceCollection_PM.h"
#include "CMachineCollection_PM.h"
#include "CWorkCenterCollection_PM.h"

#define LINE_LENGTH 512

enum MEASURE_UNIT {muSecond, muMinute, muHour, muDay};

//------------------------------------------------------------------------------
//	ClassName  : CHndFile_PM
//
//  Description: Manipula un fichero con informacion referente a una instancia
//							 del Job Shop Scheduling Problem.
// 
//  Revision   : 13/06/2014 
//------------------------------------------------------------------------------

class CHndFile_PM {
	private:
		char *sFilename;
		char KeyWord[13][16];

	private:
		bool isKeyWord (const char *string);

	public:
		//--- Constructors and Destructor ---//
		 CHndFile_PM ( void );							
		 CHndFile_PM ( char *path );	
		~CHndFile_PM ( void );							

		//--- Set functions ---//
		void SetPath ( char *path );

		//--- Get functions ---//
		const char *GetPath ( void ) const;

		//--- File handler functions ---//
		int ReadPieceCount		 ( void );
		int ReadMachineCount	 ( void );
		int ReadInstanceSize	 ( int *job, int *machine );		
    int ReadMachineData		 ( CMachineCollection_PM *m_collection );
		int ReadWorkCenterData ( CWorkCenterCollection *wc_collection );
		int ReadPieceData			 ( CPieceCollection_PM *p_collection, const CMachineCollection_PM &m_collection );
    int WriteData					 ( const CMachineCollection_PM &MachineList, const CPieceCollection_PM &Recipient );
};

#endif