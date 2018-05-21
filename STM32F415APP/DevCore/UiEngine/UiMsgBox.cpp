//******************************************************************************
//  @file UiEngine.cpp
//  @author Nicolai Shlapunov
//
//  @details DevCore: UI Message Box Class, implementation
//
//  @copyright Copyright (c) 2016, Devtronic & Nicolai Shlapunov
//             All rights reserved.
//
//  @section SUPPORT
//
//   Devtronic invests time and resources providing this open source code,
//   please support Devtronic and open-source hardware/software by
//   donations and/or purchasing products from Devtronic.
//
//******************************************************************************

// *****************************************************************************
// ***   Includes   ************************************************************
// *****************************************************************************
#include "UiEngine.h"

// *****************************************************************************
// ***   Public: MsgBox constructor   ******************************************
// *****************************************************************************
UiMsgBox::UiMsgBox(const char* msg_in, const char* hdr_in,
                   String::FontType msg_fnt_in, String::FontType hdr_fnt_in,
                   uint16_t center_x_in, uint16_t center_y_in,
                   uint16_t width_in, uint16_t color_in)
{
  // Save input params
  msg = msg_in;            // Message
  msg_fnt = msg_fnt_in;    // Message font
  hdr = hdr_in;            // Header string
  hdr_fnt = hdr_fnt_in;    // Header font
  center_x = center_x_in;  // X position of MsgBox center
  center_y = center_y_in;  // Y position of MsgBox center
  width = width_in;        // MsgBox width
  color = color_in;        // Color

  // ���������� ��� ������
  uint8_t i;
  // ���������� ��� �������� �������� ����
  int16_t X, Y, W, H, StrW;
  // ��������������� ���������
  char* p;
  // ������ ���������� �� ������
  char* line[MAX_MSGBOX_LINES];
  // ������ ����� �����
  uint8_t length[MAX_MSGBOX_LINES];
  // ������� ���������� �����
  uint8_t count = 0;

  // ��������
  if(msg == nullptr) return;                       // ���� �� �������� �������� ������ - �������
  if(msg_fnt == String::FONTS_MAX) msg_fnt = String::FONT_8x12;    // ���� �� �������� ����� - ������������� �� ���������
  if(hdr_fnt == String::FONTS_MAX) hdr_fnt = String::FONT_4x6; // ���� �� �������� ����� - ������������� �� ���������

  // �������� ������ � �����, ������� ����� ������ �� ������
  strcpy(str_buf, msg);
  
  // ������������� ������ ������ �� ������ ������
  line[count] = str_buf;

  // �������� ������ ���������
  H = 0;
  // ������ ������ ���������
  W = String::GetFontW(msg_fnt) * width;

  // � ���� ����� ����� ����� �� ������
  while(count < MAX_MSGBOX_LINES)
  {
    // ������������� ��������� p �� ����� � ������ ���������� ������ ��������� ������� '\n'
    p = strchr(line[count], (int)'\n');

    // ���� ����� ������ � ������ �� ������
    if(p == nullptr)
    {
      // ������������� ������ ������ � ������� strlen
      length[count] = strlen(line[count]);
    }
    else // �����
    {
      // ��������� ������ ������ �� �������� ����������
      length[count] = p - line[count];
      // � ������ ������ '\n' �� '\0', ����� ������� ���������� ������
      *p = '\0';
    }

    // ��������� ������ ������ � ��������
    StrW = String::GetFontW(msg_fnt) * length[count];
    // ���� ������ ������� ������ ������ ���������� - ���������� �
    if(W < StrW) W = StrW;
    // ��������� ������ ������
    H += String::GetFontH(msg_fnt);

    // ���� ������ '\n' �� ��� ������ - ��� ������ ��������� - ������� �� �����
    if(p == nullptr) break;

    // ����������� ������� �����
    count++;
    // ������������� ��������� �� ������ ��������� ������
    line[count] = p + 1;
  }

  if(hdr != nullptr)
  {
    // ��������� ������ ��������� � ��������
    StrW = String::GetFontW(hdr_fnt) * strlen(hdr);
    // ���� ������ ��������� ������ ������ �������������� ����� - ���������� �
    if(W < StrW) W = StrW;
  }

  // ��������� � �������� ���� ������ ����� ������ � ����� ������� � 1 ������
  W += String::GetFontW(msg_fnt) * 2;
  // ��������� � �������� ���� ������ ����� ������ � ����� ������� � 0.5 �������
  H += String::GetFontH(msg_fnt);

  // ��������� ��������� ���� ������������ ����������� ������
  X = center_x - W / 2;
  // ��������� ��������� ���� ������������ ����������� ������
  Y = center_y - H / 2;

  // ���������, ���� ������������
  if(hdr != nullptr)
  {
    // �������� ���� ���� �� �������� �������� ���������
    Y += String::GetFontH(hdr_fnt) / 2;
    // ������� ����� ������ ���������
    box[box_cnt++].SetParams(X - 1, Y - String::GetFontH(hdr_fnt) - 2, W + 1, String::GetFontH(hdr_fnt) + 3, COLOR_MAGENTA, false);
    // ������� ����� ��� ���������
    box[box_cnt++].SetParams(X, Y - String::GetFontH(hdr_fnt) - 1, W, String::GetFontH(hdr_fnt) + 1, COLOR_MAGENTA, true);
    // ������� ���������
    string[str_cnt++].SetParams(hdr, X + 1, Y - String::GetFontH(hdr_fnt), COLOR_YELLOW, hdr_fnt);
  }

  // ������� ����� ��� �������� ���������. �������� ����� �� �����
  // � 1 �������, ��� �� �� ����� ���������� �� ����� ����
  box[box_cnt++].SetParams(X - 1, Y - 1, W + 2, H + 2, COLOR_BLACK, false);
  // ������ ����� ������ ��������� ���������
  box[box_cnt++].SetParams(X, Y, W, H, COLOR_MAGENTA, false);

  // � ���� ����� ������� ������������ ������ �� �����
  for(i = 0; i < count + 1; i++)
  {
    // ���� ������ ������ ����� 0 - ���������� ���� ���������
    if(length[i] != 0)
    {
      // ������� ��������� �� ��� X ��� ������ ������
      X = center_x - (String::GetFontW(msg_fnt) * length[i]) / 2;
      // ������� ������
      string[str_cnt++].SetParams(line[i], X, Y + String::GetFontH(msg_fnt)/2 + i * String::GetFontH(msg_fnt), COLOR_YELLOW, msg_fnt);
    }
  }
}

// *************************************************************************
// ***   Public: Destructor   **********************************************
// *************************************************************************
UiMsgBox::~UiMsgBox()
{
  // Hide before destruct for memory clean up
  Hide();
}

// *****************************************************************************
// ***   Show MsgBox   *********************************************************
// *****************************************************************************
void UiMsgBox::Show(uint32_t z)
{
  for(uint32_t i = 0; i < box_cnt; i++)
  {
    box[i].Show(z);
  }
  for(uint32_t i = 0; i < str_cnt; i++)
  {
    string[i].Show(z + 1U);
  }
}

// *****************************************************************************
// ***   Hide MsgBox   *********************************************************
// *****************************************************************************
void UiMsgBox::Hide(void)
{
  // Delete boxes
  for(uint32_t i = 0; i < box_cnt; i++)
  {
    box[i].Hide();
  }
  // Delete strings
  for(uint32_t i = 0; i < str_cnt; i++)
  {
    string[i].Hide();
  }
}

// *****************************************************************************
// ***   Show and Hide Msg box after pause   ***********************************
// *****************************************************************************
void UiMsgBox::Run(uint32_t delay)
{
  Show();
  DisplayDrv::GetInstance().UpdateDisplay();
  vTaskDelay(delay);
  Hide();
}
