object Form1: TForm1
  Left = 462
  Top = 146
  Width = 248
  Height = 390
  Caption = 'test - Delphi 5 direct'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 24
    Top = 32
    Width = 75
    Height = 25
    Caption = 'measure'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Memo1: TMemo
    Left = 24
    Top = 80
    Width = 185
    Height = 249
    ReadOnly = True
    TabOrder = 1
  end
end
