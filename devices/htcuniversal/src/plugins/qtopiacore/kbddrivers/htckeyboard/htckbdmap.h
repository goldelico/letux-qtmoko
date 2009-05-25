/*
 *  Jeremy Compostella <jeremy.compostella@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef HTCKDBMAP_H
#define HTCKDBMAP_H

typedef struct QWSKeyMap {
  uint key_code;
  ushort unicode;
  ushort shift_unicode;
  ushort ctrl_unicode;
  ushort alt_unicode; /* Fn key modifier */
};

static const QWSKeyMap htcuniversalKeyMap[] = {
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Escape,       0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_1,                '1'     , '!'     ,   0xffff,    '!' },
  { Qt::Key_2,                '2'     , '@'     ,   0xffff,    '@' },
  { Qt::Key_3,                '3'     , '#'     ,   0xffff,    '#' },
  { Qt::Key_4,                '4'     , '$'     ,   0xffff,    '$' },
  { Qt::Key_5,                '5'     , '%'     ,   0xffff,    '%' },
  { Qt::Key_6,                '6'     , '^'     ,   0xffff,    '^' },
  { Qt::Key_7,                '7'     , '&'     ,   0xffff,    '&' },
  { Qt::Key_8,                '8'     , '*'     ,   0xffff,    '*' },
  { Qt::Key_9,                '9'     , '('     ,   0xffff,    '(' },
  { Qt::Key_0,                '0'     , ')'     ,   0xffff,    ')' },
  { Qt::Key_Minus,            '-'     , '_'     ,   0xffff,    '-' },
  { Qt::Key_Equal,            '='     , '+'     ,   0xffff,    '_' },
  { Qt::Key_Backspace,         8      ,  8      ,   0xffff,      8 },
  { Qt::Key_Tab,               9      , '~'     ,   0xffff,    '~' },
  { Qt::Key_Q,                'q'     , 'Q'     , 'Q' - 64, 0xffff },
  { Qt::Key_W,                'w'     , 'W'     , 'W' - 64, 0xffff },
  { Qt::Key_E,                'e'     , 'E'     , 'E' - 64, 0xffff },
  { Qt::Key_R,                'r'     , 'R'     , 'R' - 64, 0xffff },
  { Qt::Key_T,                't'     , 'T'     , 'T' - 64, 0xffff },
  { Qt::Key_Y,                'y'     , 'Y'     , 'Y' - 64, 0xffff },
  { Qt::Key_U,                'u'     , 'U'     , 'U' - 64, 0xffff },
  { Qt::Key_I,                'i'     , 'I'     , 'I' - 64, 0xffff },
  { Qt::Key_O,                'o'     , 'O'     , 'O' - 64,    '/' },
  { Qt::Key_P,                'p'     , 'P'     , 'P' - 64,   '\\' },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Enter,        0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Control,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_A,                'a'     , 'A'     , 'A' - 64, 0xffff },
  { Qt::Key_S,                's'     , 'S'     , 'S' - 64, 0xffff },
  { Qt::Key_D,                'd'     , 'D'     , 'D' - 64, 0xffff },
  { Qt::Key_F,                'f'     , 'F'     , 'F' - 64, 0xffff },
  { Qt::Key_G,                'g'     , 'G'     , 'G' - 64, 0xffff },
  { Qt::Key_H,                'h'     , 'H'     , 'H' - 64, 0xffff },
  { Qt::Key_J,                'j'     , 'J'     , 'J' - 64, 0xffff },
  { Qt::Key_K,                'k'     , 'K'     , 'K' - 64,    '"' },
  { Qt::Key_L,                'l'     , 'L'     , 'L' - 64,   '\'' },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Shift,        0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Z,                'z'     , 'Z'     , 'Z' - 64, 0xffff },
  { Qt::Key_X,                'x'     , 'X'     , 'X' - 64, 0xffff },
  { Qt::Key_C,                'c'     , 'C'     , 'C' - 64, 0xffff },
  { Qt::Key_V,                'v'     , 'V'     , 'V' - 64, 0xffff },
  { Qt::Key_B,                'b'     , 'B'     , 'B' - 64, 0xffff },
  { Qt::Key_N,                'n'     , 'N'     , 'N' - 64, 0xffff },
  { Qt::Key_M,                'm'     , 'M'     , 'M' - 64,    '?' },
  { Qt::Key_Comma,            ','     , ';'     ,   0xffff,    ';' },
  { Qt::Key_Period,           '.'     , ':'     ,   0xffff,    ':' },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Shift,        0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Alt,          0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Space,            ' '     , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_CapsLock,     0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Context1,     0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_F32,          0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Call,         0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Back,         0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Hangup,       0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Slash,            '/'     , '\\'    ,   '?'   , 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Call,         0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Hangup,       0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Alt,          0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Up,           0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Left,         0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Right,        0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_unknown,      0xffff      , 0xffff  ,   0xffff, 0xffff },
  { Qt::Key_Down,         0xffff      , 0xffff  ,   0xffff, 0xffff },
};

static const int keyMSize = sizeof(htcuniversalKeyMap) / sizeof(QWSKeyMap) - 1;

#endif /* HTCKDBMAP_H */
