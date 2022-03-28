#ifndef definitionsH
#define definitionsH

#include <vcl.h>

typedef struct {
  TColor Color;
  unsigned Gene;
} REGISTER_ITEM;

//==========================================================================//
//                                                                          //
//                      CLASE CRegisterItems                                //
//                                                                          //
//  Description: . //
//                                                                          //
//==========================================================================//

class CRegisterItems{
  private:
    static REGISTER_ITEM *VisorItems;
    static bool is_register_items;

  public:
    inline static bool isRegisterItems       ( void ) {return CRegisterItems::is_register_items;}
    inline static void Free                  ( void );
    inline static      REGISTER_ITEM *GetReg ( void );
    inline static void RegSequence           ( const unsigned *gene, int itemCount );
};
REGISTER_ITEM *CRegisterItems::VisorItems        = NULL;
bool           CRegisterItems::is_register_items = false;

//====================== DEFINITIONs METHODs =============================//
/*  */
inline static void CRegisterItems::Free (void)
{
  CRegisterItems::is_register_items = false;
  delete []CRegisterItems::VisorItems;
}
//------------------------------------------------------------------------

/*  */
inline static REGISTER_ITEM *CRegisterItems::GetReg (void)
{
  return CRegisterItems::VisorItems;
}
//------------------------------------------------------------------------

/*  */
inline static void CRegisterItems::RegSequence (const unsigned *gene, int itemCount)
{
  const TColor PrimaryColor[] = {clMaroon, clSkyBlue, clOlive, clGreen, clBlue, clRed,
                                 clGray, clTeal, clNavy, clPurple, clMoneyGreen,
                                 clFuchsia, clYellow, clLtGray, clAqua};

  CRegisterItems::is_register_items = true;
  if (!CRegisterItems::VisorItems) delete CRegisterItems::VisorItems;
  CRegisterItems::VisorItems = new REGISTER_ITEM[itemCount];
  srand(time(NULL));

  //=== Registro los IDs en el visor ===//
  for(int i = 0; i < itemCount; i++)
  {
    CRegisterItems::VisorItems[i].Gene = gene[i];

    if (i < 14 )
      CRegisterItems::VisorItems[i].Color = PrimaryColor[i];
    else
      // Genero valores aleatorios para las intensidades de Rojo, Verde y Azul.
      //CRegisterItems::VisorItems[i].Color = TColor(RGB(rand() % 255, rand() % 255, rand() % 255));
      CRegisterItems::VisorItems[i].Color = TColor(RGB(rand() / 35, rand() / 35, rand() / 35));
  }
}

#endif



