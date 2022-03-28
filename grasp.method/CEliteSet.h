#pragma once
#ifndef CEliteSet_H
#define CEliteSet_H

#include "../jssp.model/CSchedule.h"

//------------------------------------------------------------------------------
//	ClassName: CEliteSet
//
//  Description: Represent a elite set or memory mechanism for a GRASP metaheuristic.
//
//  Revision: 25/05/2019 
//------------------------------------------------------------------------------
class CEliteSet 
{
	private:
		typedef struct Item {
			float f;
			CSchedule S;
		} ELITE_ITEM;

	private:
		int Index;
		float fBest;
		float fWorst;
		float GanmaDiff;
		int indexBest; // Nota: (13-06-2019) Verificar que no hacen falta los datos miembros indexBest y indexWorst
		int indexWorst;
		CSchedule sBest;
		CSchedule sWorst;
		unsigned int uSize;
		ELITE_ITEM *EliteArray;

	private:
		float DeltaDiff		 ( const CSchedule &S1, const CSchedule &S2 );
		float GanmaMinFor	 ( const CSchedule &S, float f );
		int ArgMinForGanma ( const CSchedule &S, float f );
		void UpdateBestAndWorst_Index ( void );

	public:
		//--- Constructor and destructor ---//
		 CEliteSet ( unsigned int size ) ;
		~CEliteSet ( void );

		//--- Get functions ---//
		int UpdateElite ( const CSchedule &S, float f);

		//--- General purpose functions ---//
		void Restart  ( void );
		void ClearAll ( void );
		int	 Count		( void ) const;
		bool isEmpty	( void ) const;

		CSchedule RandomSelect ( float *f ) const;
		CSchedule getBest			 ( float *f = nullptr ) const;
		CSchedule getWorst		 ( float *f = nullptr ) const;
};

#endif