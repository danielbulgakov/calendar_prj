using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace _calendar_prj
{
    class Calendar
    {
        
        private enum EventColors // для рандомного выбора цвета
        {
            Aquamarine,
            Azure,
            BurlyWood,
            CadetBlue,
            Gainsboro,
            Gold,
            Gray,
            Khaki,
          
           
            MediumOrchid,
            MediumPurple,
            MistyRose,
         
            Orange,
            OrangeRed,
           
            PaleTurquoise,
            
            RoyalBlue,
            SandyBrown,
            
        };
       
        

        private readonly int[] days =
            {0,31, 28, 31, 30, 31, 30, 31, 31,
             30, 31, 30, 31};
        private readonly int[] wdays =
            {0,31, 29, 31, 30, 31, 30, 31, 31,
             30, 31, 30, 31};
        private readonly string[] months =
            {"", "Январь", "Февраль", "Март", "Апрель", "Май", "Июнь",
              "Июль", "Август", "Сентябрь", "Октябрь" , "Ноябрь", "Декабрь"};
        int x, y; // начальные координаты 
        int w, h; // Размеры
        int m;

        int FontSize = 35;  // Размер шрифта
        string FontName = "Arial"; // Названия шрифта (используется для поиска ширины строки)
        System.Drawing.Color color ;

        int Xday_pos, Xmonth_pos, Xyear_pos;

        int day;
        int year;  // Поля календаря
        string month;

        public Calendar(int _x, int _y, int _w, int _h, int fsize, System.Drawing.Color _color)
        {
            this.x = _x;
            this.y = _y;
            this.w = _w;
            this.h = _h;
            color = _color;
            FontSize = fsize;
            SetTimNow();
            SetPos();
        }


        internal void SetTimNow()
        {
            //day = DateTime.Today.Day;
            //month = months[DateTime.Today.Month - 1];
            //year = DateTime.Today.Year;
            day = 30;
            month = months[12];
            year = 2021;
        }

        internal void SetPos()
        {
            Xday_pos = x;
            Xmonth_pos = x + StringCount(day.ToString());
            Xyear_pos = x + StringCount(day.ToString()) + StringCount(month) + 25;
        }

        internal int isInsideSmth(int xm, int ym)
        {
            // -1 - где-то не тут 
            // 0 - внутри значения дня
            // 1 - внутри значения месяца
            // 2 - внутри значения года
            if (ym > y + 10 && ym < y + FontSize + 10)
            {
                if (xm < Xmonth_pos && xm > x) return 0;
                else if (xm < Xyear_pos && xm >= Xmonth_pos) return 1;
                else if (xm < Xyear_pos + StringCount(year.ToString()) &&
                    xm >= Xyear_pos) return 2;
                else return -1;
            }
            else return -1;

        }
        internal bool YearWiskas(int _year) 
        {
            if ((_year%4 == 0 && _year % 100 != 0) || _year % 400 == 0) return true;
            else return false;
	    
        }


        internal int StringCount(string a)
        {
            Font font = new Font(FontName, FontSize);
            SizeF stringSize = new SizeF();
            Graphics gfx = Graphics.FromImage(new Bitmap(1, 1));
            stringSize = gfx.MeasureString(a, font);
            int width = (int)stringSize.Width;
            
            return (width + 10);

        }

        internal void Paint(Graphics g)
        {
           

            Font font = new Font(FontName, FontSize);
            SolidBrush brush = new SolidBrush(color);

            g.DrawString(day.ToString(),
                font, brush, Xday_pos, y);

            g.DrawString(month, font, brush, Xmonth_pos, y);

            g.DrawString(year.ToString(),
                font, brush, Xyear_pos, y);

        }

        internal int CheckDay(int _day, int sign)
        {
            if (YearWiskas(year))
            {
                if (sign == 1)
                {
                    if (_day < wdays[Array.IndexOf(months, month)])
                        return ++_day;
                    else return 1;

                }
                if (sign == -1)
                {
                    if (_day > 1) return --_day;
                    else return wdays[Array.IndexOf(months, month)];
                }
                return 0;
            }
            else
            {
                if (sign == 1)
                {
                    if (_day < days[Array.IndexOf(months, month)])
                        return ++_day;
                    else return 1;

                }
                if (sign == -1)
                {
                    if (_day > 1) return --_day;
                    else return days[Array.IndexOf(months, month) ];
                }
                return 0;
            }
        }

        internal string CheckMonth(string month, int sign)
        {
            if (sign == 1)
            {
                if (Array.IndexOf(months, month) < 12)
                    return months[Array.IndexOf(months, month)+1];
                else return months[1];
            }
            if (sign == -1)
            {
                if (Array.IndexOf(months, month) > 1) return months[Array.IndexOf(months, month) - 1];
                else return months[12];
            }
            return  "0";
        }

        internal void DataCheck(int pe, int Delta)
        {


            if (pe == 0)
            {
                if (Delta > 0)
                {
                    CheckDay(day, 1);
                    if (CheckDay(day, 1) == 1) CheckMonth(month, 1);
                    if (months[1] == month) year++;
                }
                if (Delta < 0)
                {
                    CheckDay(day, -1);
                    if (CheckDay(day, -1) == days[Array.IndexOf(months, month)]) CheckMonth(month, -1);
                    if (months[12] == month) year--;
                }
            }
            
        }

        internal void Paint_MouseHover(int pe, Graphics g, int k)
        {
            // k - коэфициент, который показывает увеличить значения даты/месяц/год или уменьшить на 1
            // pe - возращает 0 - если указатель на дате, 1 - на месяце, 2 - год
            
            
            SolidBrush brush = new SolidBrush(Color.Black);    
            List<int> cases = new List<int> { Xday_pos, Xmonth_pos, Xyear_pos };
            Font font = new Font(FontName, FontSize - 8);
            
            
            if (pe == 0)
            {
                if (k != 0)
                {
                    day = CheckDay(day, k);
                  
                }

                int _day = day;
                
                    g.Clear(Color.White);
                    Paint(g);
               
                
                    g.DrawString((CheckDay(_day, 1)).ToString(), font, brush, cases[pe] + 12, y - FontSize - 10);
                    g.DrawString((CheckDay(_day, -1)).ToString(), font, brush, cases[pe] + 12, y + FontSize + 20);
             
                
            }
            if (pe == 1 )
            {
                
                if (k!= 0) month = CheckMonth(month, k);
                if (day > days[Array.IndexOf(months, month)])
                {
                    day = days[Array.IndexOf(months, month)];
                    
                    g.DrawString((CheckDay(day, 1)).ToString(), font, brush, cases[pe] + 12, y - FontSize - 10);
                    g.DrawString((CheckDay(day, -1)).ToString(), font, brush, cases[pe] + 12, y + FontSize + 20);
                }
                g.Clear(Color.White);
                Paint(g);
                g.DrawString((CheckMonth(month, 1 )), font , brush, cases[pe] + 12, y - FontSize - 10);
                g.DrawString((CheckMonth(month, -1)), font , brush, cases[pe] + 12, y + FontSize + 20);
               
            }
            if (pe == 2 )
            {
                year += k;
                int _year = year;
                g.Clear(Color.White);
                Paint(g);
                g.DrawString((++_year).ToString(), font , brush, cases[pe] + 12, y - FontSize - 10);
                _year = year;
                g.DrawString(((--_year)).ToString(), font , brush, cases[pe] + 12, y + FontSize + 20);
                
            }
            if ((pe == -1 && m!= pe ) )
            {
               g.Clear(Color.White);
               Paint(g);
            }
            m = pe;
           




        }
        
          

         
    
    
    } 

        
    
}
