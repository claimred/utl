#include "config.h"
#include "utils.h"

#include "game_map_preparer.h"

using namespace utl_core;
 
GameMapPreparer::GameMapPreparer( GameMap &gm ) : _gm(gm)
{
   _initConf();
}

int GameMapPreparer::_count( int x, int y, mapelem::MapElem el )
{
   int coord[8][2] = { {-1, -1}, { 0, -1}, {1, -1}, 
                       { 1,  0}, { 1,  1}, {0,  1}, 
                       {-1,  1}, {-1,  0} };
   int res = 0;

   std::pair<int, int> size;

   _gm.getSize(size);

   for (int i = 0; i < 8; i++)
   {
      int new_x = x + coord[i][0],
          new_y = y + coord[i][1];

      new_x = Utils::clamp(new_x, 0, size.first - 1);
      new_y = Utils::clamp(new_y, 0, size.second - 1);

      if (_gm.getElem(new_x, new_y) == el )
         res += 1 << i;
   }

   return res;
}

void GameMapPreparer::_prepare( mapelem::MapElem el1, mapelem::MapElem el2, bool switcher, int offset, bool special )
{
   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   int res;

   for (int j = 0; j < sizes.second; j++)
      for (int i = 0; i < sizes.first; i++)
      {
         mapelem::MapElem el = _gm.getElem(i, j);

         if (el == el2)
         {
            mapelem::MapElem tmp;
            res = _count(i, j, el1);

            // TODO: meh
            if (res != 0)
               if (switcher)
                  if (special)
                     tmp = static_cast<mapelem::MapElem>(16 * (_conf[res] - 1) + 1 + el1);
                  else
                     tmp = static_cast<mapelem::MapElem>(_conf[res] + el1);
               else
                  if (special)
                     tmp = static_cast<mapelem::MapElem>(16 * (_conf[res] - 1) + 1 + offset + el2);
                  else
                     tmp = static_cast<mapelem::MapElem>(_conf[res] + offset + el2);
                     
            else
               tmp = el2;

            _gm.setElem(i, j, tmp);
         }
      }
}

void GameMapPreparer::_initConf()
{
   using namespace mapelem;

   // Offsets configuration
   _conf[0]=OFS_0;
   _conf[1]=OFS_33;
   _conf[2]=OFS_26;
   _conf[3]=OFS_26;
   _conf[6]=OFS_26;
   _conf[7]=OFS_26;
   _conf[4]=OFS_34;
   _conf[5]=OFS_20;
   _conf[8]=OFS_24;
   _conf[9]=OFS_12;
   _conf[10]=OFS_28;
   _conf[11]=OFS_28;
   _conf[12]=OFS_24;
   _conf[13]=OFS_12;
   _conf[14]=OFS_28;
   _conf[15]=OFS_28;
   _conf[16]=OFS_31;
   _conf[17]=OFS_0;
   _conf[18]=OFS_15;
   _conf[19]=OFS_15;
   _conf[20]=OFS_21;
   _conf[21]=OFS_0;
   _conf[22]=OFS_15;
   _conf[23]=OFS_15;
   _conf[24]=OFS_24;
   _conf[25]=OFS_12;
   _conf[26]=OFS_28;
   _conf[27]=OFS_28;
   _conf[28]=OFS_24;
   _conf[29]=OFS_12;
   _conf[30]=OFS_28;
   _conf[31]=OFS_28;
   _conf[32]=OFS_25;
   _conf[33]=OFS_18;
   _conf[34]=OFS_1;
   _conf[35]=OFS_1;
   _conf[36]=OFS_16;
   _conf[37]=OFS_0;
   _conf[38]=OFS_1;
   _conf[39]=OFS_1;
   _conf[40]=OFS_29;
   _conf[41]=OFS_0;
   _conf[42]=OFS_8;
   _conf[43]=OFS_8;
   _conf[44]=OFS_29;
   _conf[45]=OFS_5;
   _conf[46]=OFS_8;
   _conf[47]=OFS_8;
   _conf[48]=OFS_25;
   _conf[49]=OFS_18;
   _conf[50]=OFS_1;
   _conf[51]=OFS_1;
   _conf[52]=OFS_16;
   _conf[53]=OFS_0;
   _conf[54]=OFS_1;
   _conf[55]=OFS_1;
   _conf[56]=OFS_29;
   _conf[57]=OFS_5;
   _conf[58]=OFS_8;
   _conf[59]=OFS_8;
   _conf[60]=OFS_29;
   _conf[61]=OFS_5;
   _conf[62]=OFS_8;
   _conf[63]=OFS_8;
   _conf[64]=OFS_32;
   _conf[65]=OFS_22;
   _conf[66]=OFS_17;
   _conf[67]=OFS_17;
   _conf[68]=OFS_0;
   _conf[69]=OFS_0;
   _conf[70]=OFS_17;
   _conf[71]=OFS_17;
   _conf[72]=OFS_11;
   _conf[73]=OFS_0;
   _conf[74]=OFS_4;
   _conf[75]=OFS_4;
   _conf[76]=OFS_11;
   _conf[77]=OFS_0;
   _conf[78]=OFS_4;
   _conf[79]=OFS_4;
   _conf[80]=OFS_19;
   _conf[81]=OFS_0;
   _conf[82]=OFS_0;
   _conf[83]=OFS_4;
   _conf[84]=OFS_0;
   _conf[85]=OFS_0;
   _conf[86]=OFS_0;
   _conf[87]=OFS_0;
   _conf[88]=OFS_11;
   _conf[89]=OFS_0;
   _conf[90]=OFS_4;
   _conf[91]=OFS_4;
   _conf[92]=OFS_11;
   _conf[93]=OFS_0;
   _conf[94]=OFS_4;
   _conf[95]=OFS_4;
   _conf[96]=OFS_25;
   _conf[97]=OFS_18;
   _conf[98]=OFS_1;
   _conf[99]=OFS_1;
   _conf[100]=OFS_16;
   _conf[101]=OFS_0;
   _conf[102]=OFS_1;
   _conf[103]=OFS_1;
   _conf[104]=OFS_29;
   _conf[105]=OFS_5;
   _conf[106]=OFS_8;
   _conf[107]=OFS_8;
   _conf[108]=OFS_29;
   _conf[109]=OFS_5;
   _conf[110]=OFS_8;
   _conf[111]=OFS_8;
   _conf[112]=OFS_25;
   _conf[113]=OFS_18;
   _conf[114]=OFS_1;
   _conf[115]=OFS_1;
   _conf[116]=OFS_16;
   _conf[117]=OFS_0;
   _conf[118]=OFS_1;
   _conf[119]=OFS_1;
   _conf[120]=OFS_29;
   _conf[121]=OFS_5;
   _conf[122]=OFS_8;
   _conf[123]=OFS_8;
   _conf[124]=OFS_29;
   _conf[125]=OFS_5;
   _conf[126]=OFS_8;
   _conf[127]=OFS_8;
   _conf[128]=OFS_23;
   _conf[129]=OFS_23;
   _conf[130]=OFS_27;
   _conf[131]=OFS_27;
   _conf[132]=OFS_14;
   _conf[133]=OFS_14;
   _conf[134]=OFS_27;
   _conf[135]=OFS_27;
   _conf[136]=OFS_2;
   _conf[137]=OFS_2;
   _conf[138]=OFS_9;
   _conf[139]=OFS_9;
   _conf[140]=OFS_2;
   _conf[141]=OFS_2;
   _conf[142]=OFS_9;
   _conf[143]=OFS_9;
   _conf[144]=OFS_13;
   _conf[145]=OFS_13;
   _conf[146]=OFS_3;
   _conf[147]=OFS_3;
   _conf[148]=OFS_0;
   _conf[149]=OFS_0;
   _conf[150]=OFS_3;
   _conf[151]=OFS_3;
   _conf[152]=OFS_2;
   _conf[153]=OFS_2;
   _conf[154]=OFS_9;
   _conf[155]=OFS_9;
   _conf[156]=OFS_2;
   _conf[157]=OFS_2;
   _conf[158]=OFS_9;
   _conf[159]=OFS_9;
   _conf[160]=OFS_30;
   _conf[161]=OFS_30;
   _conf[162]=OFS_7;
   _conf[163]=OFS_7;
   _conf[164]=OFS_6;
   _conf[165]=OFS_6;
   _conf[166]=OFS_7;
   _conf[167]=OFS_7;
   _conf[168]=OFS_10;
   _conf[169]=OFS_10;
   _conf[170]=OFS_0;
   _conf[171]=OFS_0;
   _conf[172]=OFS_10;
   _conf[173]=OFS_10;
   _conf[174]=OFS_0;
   _conf[175]=OFS_0;
   _conf[176]=OFS_30;
   _conf[177]=OFS_30;
   _conf[178]=OFS_7;
   _conf[179]=OFS_7;
   _conf[180]=OFS_6;
   _conf[181]=OFS_6;
   _conf[182]=OFS_7;
   _conf[183]=OFS_7;
   _conf[184]=OFS_10;
   _conf[185]=OFS_10;
   _conf[186]=OFS_0;
   _conf[187]=OFS_0;
   _conf[188]=OFS_10;
   _conf[189]=OFS_10;
   _conf[190]=OFS_0;
   _conf[191]=OFS_0;
   _conf[192]=OFS_23;
   _conf[193]=OFS_23;
   _conf[194]=OFS_27;
   _conf[195]=OFS_27;
   _conf[196]=OFS_14;
   _conf[197]=OFS_14;
   _conf[198]=OFS_27;
   _conf[199]=OFS_27;
   _conf[200]=OFS_2;
   _conf[201]=OFS_2;
   _conf[202]=OFS_9;
   _conf[203]=OFS_9;
   _conf[204]=OFS_2;
   _conf[205]=OFS_2;
   _conf[206]=OFS_9;
   _conf[207]=OFS_9;
   _conf[208]=OFS_13;
   _conf[209]=OFS_13;
   _conf[210]=OFS_3;
   _conf[211]=OFS_3;
   _conf[212]=OFS_0;
   _conf[213]=OFS_0;
   _conf[214]=OFS_3;
   _conf[215]=OFS_3;
   _conf[216]=OFS_2;
   _conf[217]=OFS_2;
   _conf[218]=OFS_9;
   _conf[219]=OFS_9;
   _conf[220]=OFS_2;
   _conf[221]=OFS_2;
   _conf[222]=OFS_9;
   _conf[223]=OFS_9;
   _conf[224]=OFS_30;
   _conf[225]=OFS_30;
   _conf[226]=OFS_7;
   _conf[227]=OFS_7;
   _conf[228]=OFS_6;
   _conf[229]=OFS_6;
   _conf[230]=OFS_7;
   _conf[231]=OFS_7;
   _conf[232]=OFS_10;
   _conf[233]=OFS_10;
   _conf[234]=OFS_0;
   _conf[235]=OFS_0;
   _conf[236]=OFS_10;
   _conf[237]=OFS_10;
   _conf[238]=OFS_0;
   _conf[239]=OFS_0;
   _conf[240]=OFS_30;
   _conf[241]=OFS_30;
   _conf[242]=OFS_7;
   _conf[243]=OFS_7;
   _conf[244]=OFS_6;
   _conf[245]=OFS_6;
   _conf[246]=OFS_7;
   _conf[247]=OFS_7;
   _conf[248]=OFS_10;
   _conf[249]=OFS_10;
   _conf[250]=OFS_0;
   _conf[251]=OFS_0;
   _conf[252]=OFS_10;
   _conf[253]=OFS_10;
   _conf[254]=OFS_0;
   _conf[255]=OFS_0;
}

void GameMapPreparer::_markCell( bool *conf, int i, int j, bool inv )
{
   int countx = Config::countX, county = Config::countY;
      
   for (int q = 0; q < county; q++)
      for (int p = 0; p < countx; p++)
      {
         if (inv)
            _gm.setWalkable(i * countx + p, j * county + q, !conf[q * countx + p]);
         else
            _gm.setWalkable(i * countx + p, j * county + q, conf[q * countx + p]);
      }
}

void GameMapPreparer::_markWalkable()
{
   using namespace mapelem;

   int countx = Config::countX, county = Config::countY;

   bool configs[34][16] = 
   
   { 
   
      //1 & 2
   {1, 1, 1, 1,
    0, 0, 0, 0, 
    0, 0, 0, 0, 
    1, 1, 1, 1 },

   {1, 0, 0, 1, 
    1, 0, 0, 1, 
    1, 0, 0, 1, 
    1, 0, 0, 1 },
   
    //3, 4, 5, 6
   {1, 1, 1, 1, 
    1, 1, 0, 0, 
    1, 0, 0, 1, 
    1, 0, 0, 1 },

   {1, 1, 1, 1, 
    0, 0, 1, 1, 
    0, 0, 0, 1, 
    1, 0, 0, 1 },

   {1, 0, 0, 1, 
    0, 0, 0, 1, 
    0, 0, 1, 1, 
    1, 1, 1, 1 },

   {1, 0, 0, 1, 
    1, 0, 0, 1, 
    1, 1, 0, 0, 
    1, 1, 1, 1 },

    //7, 8, 9, 10
    {1, 1, 1, 1, 
     1, 1, 1, 0, 
     1, 1, 1, 0, 
     1, 1, 1, 1 },

    {1, 1, 1, 1, 
     0, 1, 1, 1, 
     0, 1, 1, 1, 
     1, 1, 1, 1 },

    {1, 1, 1, 1, 
     1, 1, 1, 1, 
     1, 1, 1, 1, 
     1, 0, 0, 1 },

    {1, 0, 0, 1, 
     1, 1, 1, 1, 
     1, 1, 1, 1, 
     1, 1, 1, 1 },

     //11-18
    {0, 0, 1, 1, 
     0, 0, 1, 1, 
     1, 0, 0, 1, 
     1, 0, 0, 1 },

    {1, 0, 0, 1, 
     1, 0, 0, 1, 
     0, 0, 1, 1, 
     0, 0, 1, 1 },

    {1, 1, 0, 0, 
     1, 1, 0, 0, 
     1, 0, 0, 1, 
     1, 0, 0, 1 },

    {1, 0, 0, 1, 
     1, 0, 0, 1, 
     1, 1, 0, 0, 
     1, 1, 0, 0 },


    {1, 1, 1, 1, 
     1, 1, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 1, 1 },

    {0, 0, 1, 1, 
     0, 0, 0, 0, 
     1, 1, 0, 0, 
     1, 1, 1, 1 },

    {1, 1, 1, 1, 
     0, 0, 1, 1, 
     0, 0, 0, 0, 
     1, 1, 0, 0 },

    {1, 1, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 1, 1, 
     1, 1, 1, 1 },

     //19-22
    {0, 0, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 0, 0, 
     1, 0, 0, 1 },

    {1, 0, 0, 1, 
     0, 0, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 0, 0 },

    {0, 0, 1, 1, 
     0, 0, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 1, 1 },

    {1, 1, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 0, 0, 
     1, 1, 0, 0 },

    //23-26

    {1, 1, 0, 0, 
     1, 1, 0, 0, 
     1, 1, 0, 0, 
     1, 1, 0, 0 },

    {0, 0, 1, 1, 
     0, 0, 1, 1, 
     0, 0, 1, 1, 
     0, 0, 1, 1 },

    {0, 0, 0, 0, 
     0, 0, 0, 0, 
     1, 1, 1, 1, 
     1, 1, 1, 1 },

    {1, 1, 1, 1, 
     1, 1, 1, 1, 
     0, 0, 0, 0, 
     0, 0, 0, 0 },

    //27-30

    {1, 1, 1, 1, 
     1, 1, 1, 1, 
     1, 1, 1, 0, 
     1, 1, 1, 0 },

    {1, 1, 1, 1, 
     1, 1, 1, 1, 
     0, 1, 1, 1, 
     0, 1, 1, 1 },

    {0, 1, 1, 1, 
     0, 1, 1, 1, 
     1, 1, 1, 1, 
     1, 1, 1, 1 },

    {1, 1, 1, 0, 
     1, 1, 1, 0, 
     1, 1, 1, 1, 
     1, 1, 1, 1 },

    //31-34

    {0, 0, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 0, 1, 
     0, 0, 1, 1 },

    {0, 0, 0, 0, 
     0, 0, 0, 0, 
     1, 0, 0, 0, 
     1, 1, 0, 0 },

    {1, 1, 0, 0, 
     1, 0, 0, 0, 
     0, 0, 0, 0, 
     0, 0, 0, 0 },

    {0, 0, 1, 1, 
     0, 0, 0, 1, 
     0, 0, 0, 0, 
     0, 0, 0, 0 } };

   
   std::pair<int, int> sizes;

   _gm.getSize(sizes);

   for (int j = 0; j < sizes.second; j++)
      for (int i = 0; i < sizes.first; i++)
      {
         mapelem::MapElem el = _gm.getElem(i, j);

         if (el == GRASS || el == GRAVE || el == SAND ||
            (el >= G2S_1 && el <= G2S_34) ||
            (el >= GR2G_1 && el <= GR2G_34) )
         {
            for (int q = 0; q < county; q++)
               for (int p = 0; p < countx; p++)
               {
                  _gm.setWalkable(i * countx + p, j * county + q, true);
               }
         }

         if (el == WATER)
         {
            for (int q = 0; q < county; q++)
               for (int p = 0; p < countx; p++)
               {
                  _gm.setWalkable(i * countx + p, j * county + q, false);
               }
         }

         if (el >= S2W_0101 && el <= S2W_3401)
         {
            int ind = (el - S2W_0101) / 16;
            _markCell(configs[ind], i, j, false);
         }    


         if (el >= W2G_0101 && el <= W2G_3401)
         {
            int ind = (el - W2G_0101) / 16;
            _markCell(configs[ind], i, j, false);
         }
      }
}

void GameMapPreparer::prepare( bool aprepare )
{
   if (aprepare)
   {
      _prepare(mapelem::SAND, mapelem::WATER, true, -1, true);
      _prepare(mapelem::GRASS, mapelem::SAND);
      _prepare(mapelem::GRASS, mapelem::WATER, false, 15, true);
      _prepare(mapelem::GRASS, mapelem::GRAVE, false, 0, false);
   }

   _markWalkable();
}

GameMapPreparer::~GameMapPreparer()
{
}