const unsigned char COLOR_TABLE[256][3]={{255, 255, 255},
  {  0,   0, 255},
  {  0, 255,   0},
  {255,   0,   0},
  {255, 255,   0},
  {255,   0, 255},
  {255, 127,   0},
  {  0, 255, 255},
  { 64,  64,  64},
  {192, 192, 192},
  {254,   0,  96},
  {160, 224,   0},
  {  0, 254, 160},
  {128,   0, 160},
  {176, 176, 176},
  {  0, 240, 240},
  {240, 240, 240},
  {  0,   0, 240},
  {  0, 240,   0},
  {240,   0,   0},
  {240, 240,   0},
  {240,   0, 240},
  {240, 122,   0},
  {  0, 240, 240},
  {240, 240, 240},
  {  0,   0, 240},
  {  0, 240,   0},
  {240,   0,   0},
  {240, 240,   0},
  {240,   0, 240},
  {240, 122,   0},
  {  0, 225, 225},
  {225, 225, 225},
  {  0,   0, 225},
  {  0, 225,   0},
  {225,   0,   0},
  {225, 225,   0},
  {225,   0, 225},
  {225, 117,   0},
  {  0, 225, 225},
  {225, 225, 225},
  {  0,   0, 225},
  {  0, 225,   0},
  {225,   0,   0},
  {225, 225,   0},
  {225,   0, 225},
  {225, 117,   0},
  {  0, 210, 210},
  {210, 210, 210},
  {  0,   0, 210},
  {  0, 210,   0},
  {210,   0,   0},
  {210, 210,   0},
  {210,   0, 210},
  {210, 112,   0},
  {  0, 210, 210},
  {210, 210, 210},
  {  0,   0, 210},
  {  0, 210,   0},
  {210,   0,   0},
  {210, 210,   0},
  {210,   0, 210},
  {210, 112,   0},
  {  0, 195, 195},
  {195, 195, 195},
  {  0,   0, 195},
  {  0, 195,   0},
  {195,   0,   0},
  {195, 195,   0},
  {195,   0, 195},
  {195, 107,   0},
  {  0, 195, 195},
  {195, 195, 195},
  {  0,   0, 195},
  {  0, 195,   0},
  {195,   0,   0},
  {195, 195,   0},
  {195,   0, 195},
  {195, 107,   0},
  {  0, 180, 180},
  {180, 180, 180},
  {  0,   0, 180},
  {  0, 180,   0},
  {180,   0,   0},
  {180, 180,   0},
  {180,   0, 180},
  {180, 102,   0},
  {  0, 180, 180},
  {180, 180, 180},
  {  0,   0, 180},
  {  0, 180,   0},
  {180,   0,   0},
  {180, 180,   0},
  {180,   0, 180},
  {180, 102,   0},
  {  0, 165, 165},
  {165, 165, 165},
  {  0,   0, 165},
  {  0, 165,   0},
  {165,   0,   0},
  {165, 165,   0},
  {165,   0, 165},
  {165,  97,   0},
  {  0, 165, 165},
  {165, 165, 165},
  {  0,   0, 165},
  {  0, 165,   0},
  {165,   0,   0},
  {165, 165,   0},
  {165,   0, 165},
  {165,  97,   0},
  {  0, 150, 150},
  {150, 150, 150},
  {  0,   0, 150},
  {  0, 150,   0},
  {150,   0,   0},
  {150, 150,   0},
  {150,   0, 150},
  {150,  92,   0},
  {  0, 150, 150},
  {150, 150, 150},
  {  0,   0, 150},
  {  0, 150,   0},
  {150,   0,   0},
  {150, 150,   0},
  {150,   0, 150},
  {150,  92,   0},
  {  0, 135, 135},
  {135, 135, 135},
  {  0,   0, 135},
  {  0, 135,   0},
  {135,   0,   0},
  {135, 135,   0},
  {135,   0, 135},
  {135,  87,   0},
  {  0, 135, 135},
  {135, 135, 135},
  {  0,   0, 135},
  {  0, 135,   0},
  {135,   0,   0},
  {135, 135,   0},
  {135,   0, 135},
  {135,  87,   0},
  {  0, 120, 120},
  {120, 120, 120},
  {  0,   0, 120},
  {  0, 120,   0},
  {120,   0,   0},
  {120, 120,   0},
  {120,   0, 120},
  {120,  82,   0},
  {  0, 120, 120},
  {120, 120, 120},
  {  0,   0, 120},
  {  0, 120,   0},
  {120,   0,   0},
  {120, 120,   0},
  {120,   0, 120},
  {120,  82,   0},
  {  0, 105, 105},
  {105, 105, 105},
  {  0,   0, 105},
  {  0, 105,   0},
  {105,   0,   0},
  {105, 105,   0},
  {105,   0, 105},
  {105,  77,   0},
  {  0, 105, 105},
  {105, 105, 105},
  {  0,   0, 105},
  {  0, 105,   0},
  {105,   0,   0},
  {105, 105,   0},
  {105,   0, 105},
  {105,  77,   0},
  {  0,  90,  90},
  { 90,  90,  90},
  {  0,   0,  90},
  {  0,  90,   0},
  { 90,   0,   0},
  { 90,  90,   0},
  { 90,   0,  90},
  { 90,  72,   0},
  {  0,  90,  90},
  { 90,  90,  90},
  {  0,   0,  90},
  {  0,  90,   0},
  { 90,   0,   0},
  { 90,  90,   0},
  { 90,   0,  90},
  { 90,  72,   0},
  {  0,  75,  75},
  { 75,  75,  75},
  {  0,   0,  75},
  {  0,  75,   0},
  { 75,   0,   0},
  { 75,  75,   0},
  { 75,   0,  75},
  { 75,  67,   0},
  {  0,  75,  75},
  { 75,  75,  75},
  {  0,   0,  75},
  {  0,  75,   0},
  { 75,   0,   0},
  { 75,  75,   0},
  { 75,   0,  75},
  { 75,  67,   0},
  {  0,  60,  60},
  { 60,  60,  60},
  {  0,   0,  60},
  {  0,  60,   0},
  { 60,   0,   0},
  { 60,  60,   0},
  { 60,   0,  60},
  { 60,  62,   0},
  {  0,  60,  60},
  { 60,  60,  60},
  {  0,   0,  60},
  {  0,  60,   0},
  { 60,   0,   0},
  { 60,  60,   0},
  { 60,   0,  60},
  { 60,  62,   0},
  {  0,  45,  45},
  { 45,  45,  45},
  {  0,   0,  45},
  {  0,  45,   0},
  { 45,   0,   0},
  { 45,  45,   0},
  { 45,   0,  45},
  { 45,  57,   0},
  {  0,  45,  45},
  { 45,  45,  45},
  {  0,   0,  45},
  {  0,  45,   0},
  { 45,   0,   0},
  { 45,  45,   0},
  { 45,   0,  45},
  { 45,  57,   0},
  {  0,  30,  30},
  { 30,  30,  30},
  {  0,   0,  30},
  {  0,  30,   0},
  { 30,   0,   0},
  { 30,  30,   0},
  { 30,   0,  30},
  { 30,  52,   0},
  {  0,  30,  30},
  { 30,  30,  30},
  {  0,   0,  30},
  {  0,  30,   0},
  { 30,   0,   0},
  { 30,  30,   0},
  { 30,   0,  30},
  {192, 192, 192},
  {  0,   0,   0}};