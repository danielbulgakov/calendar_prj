using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;




namespace _calendar_prj
{

    public partial class Form1 : Form
    {
        Calendar Obj1, Obj2;
        int k = 0;
        int mouse_x = 0;
        int mouse_y = 0;
        


        public Form1( )
        {
            
            
            



            InitializeComponent();
            this.MouseWheel += wheelctrl;
            this.MouseMove += MouseCtrl;
            this.Width = 900;
            this.Height = 400;

            Obj1 = new Calendar(50, 75, 50, 100, 35, Color.DarkGreen);
            Obj2 = new Calendar(85, 250, 100, 150, 45, Color.Indigo);




        }
                
            

        
        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            
            Graphics g = this.CreateGraphics();
            Obj1.Paint(g);
            Obj2.Paint(g);

        }


       

        private void MouseCtrl(object sender, System.Windows.Forms.MouseEventArgs e )
        {
            
            //while (ClientRectangle.Contains(PointToClient(MousePosition)))

            
            mouse_x = Control.MousePosition.X - this.Location.X - 8;
            mouse_y = Control.MousePosition.Y - this.Location.Y - 30;
              
                
               
            Graphics g = this.CreateGraphics();
            int wheel = e.Delta;
            Task task1 = Task.Run(() =>
            {
                Obj1.DataCheck(Obj1.isInsideSmth(mouse_x, mouse_y), wheel);
                Obj2.DataCheck(Obj2.isInsideSmth(mouse_x, mouse_y), wheel);
            });
            Task task2 = Task.Run(() =>
            {
                Obj1.Paint_MouseHover(Obj1.isInsideSmth(mouse_x, mouse_y), g, k);                
                Obj2.Paint_MouseHover(Obj2.isInsideSmth(mouse_x, mouse_y), g, k);
                
        
            
                Obj1.Paint(g);
                Obj2.Paint(g);
                
            });



            task1.Wait();
            task2.Wait();
           
            Thread.Sleep(55);
            
            k = 0;
            
        }

      

        private void wheelctrl(object sender, MouseEventArgs e)
        {
            
            if (e.Delta > 0) k = 1;
            else if (e.Delta < 0) k = -1;
            
            
        }
      



        private void MainForm_FormClosed(object sender, EventArgs e)
        {
            System.Environment.Exit(1);
        }

        



    }
}
