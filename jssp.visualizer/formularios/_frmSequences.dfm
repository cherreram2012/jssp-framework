object frmSequences: TfrmSequences
  Left = 195
  Top = 119
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Secuencias est'#225'ticas'
  ClientHeight = 308
  ClientWidth = 794
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 7
    Width = 294
    Height = 13
    Caption = 'Entre las secuencias a reprensentar en un Diagrama de Gantt:'
  end
  object Label2: TLabel
    Left = 11
    Top = 290
    Width = 449
    Height = 13
    Caption = 
      'Nota: Utilizar numeros separados por espacios para representar l' +
      'as secuencias'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object strgrdSequence: TStringGrid
    Left = 3
    Top = 23
    Width = 622
    Height = 262
    ColCount = 1
    DefaultColWidth = 598
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 1
    FixedRows = 0
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goEditing, goAlwaysShowEditor]
    ParentFont = False
    TabOrder = 0
    OnKeyPress = strgrdSequenceKeyPress
  end
  object btnNew: TBitBtn
    Left = 637
    Top = 24
    Width = 78
    Height = 25
    Caption = 'Nueva'
    TabOrder = 1
    OnClick = btnNewClick
    Glyph.Data = {
      76030000424D7603000000000000360000002800000011000000100000000100
      1800000000004003000000000000000000000000000000000000C8D0D4C8D0D4
      C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4
      C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0
      D400C8D0D4C8D0D4C8D0D4000000C8D0D4000000C8D0D4000000C8D0D4000000
      C8D0D4000000C8D0D4000000C8D0D4C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4C8
      D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4
      000000C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4000000C8D0D4C8D0D4C8D0D4C8
      D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D400C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8
      D0D4C8D0D4C8D0D4000000C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4000000C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8
      D0D4C8D0D400C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4808080C8D0D4C8D0D4C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4000000C8D0D4C8D0D400C8D0D4C8D0D4
      808080000000C8D0D480808000FFFFC8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D400C8D0D4C8D0D4C8D0D400FFFFC8D0D4808080
      C8D0D400FFFF808080C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4000000C8D0D4C8D0
      D400C8D0D4C8D0D4808080808080808080C8D0D400FFFF00FFFFC8D0D4C8D0D4
      C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D400C8D0D4C8D0D4C8D0D400
      FFFF80808000FFFFFFFFFF808080808080808080808080C8D0D4C8D0D4C8D0D4
      000000C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4808080C8D0D480808000FFFF80
      808000FFFF000000C8D0D4000000C8D0D4000000C8D0D4C8D0D4C8D0D400C8D0
      D4C8D0D4808080C8D0D4C8D0D4808080FFFFFFC8D0D480808000FFFFC8D0D4C8
      D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D400C8D0D4C8D0D4C8D0D4C8D0D4C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8
      D0D4C8D0D400C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0
      D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D4C8D0D400}
  end
  object btnAcept: TBitBtn
    Left = 634
    Top = 260
    Width = 73
    Height = 25
    Caption = 'Aceptar'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = btnAceptClick
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
  end
  object btnCancel: TBitBtn
    Left = 714
    Top = 260
    Width = 73
    Height = 25
    Caption = 'Cancelar'
    TabOrder = 4
    OnClick = btnCancelClick
    Kind = bkNo
  end
  object GroupBox1: TGroupBox
    Left = 636
    Top = 72
    Width = 150
    Height = 65
    Caption = ' Energ'#237'a: '
    TabOrder = 2
    object Label3: TLabel
      Left = 8
      Top = 18
      Width = 113
      Height = 31
      Caption = 'Cantidad de Lecturas:'
      WordWrap = True
    end
    object Edit1: TEdit
      Left = 9
      Top = 36
      Width = 48
      Height = 21
      TabOrder = 0
      Text = '10'
    end
  end
end
