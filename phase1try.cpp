#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
using namespace std;

ifstream fin;
ofstream fout;
int SI;


class memory
{
    private:
        char mem[100][4];
        char ch;
        string IR,R;
        int IC;
        bool C;
       
    public:
        void reset()
        {
            memset(mem,'$',sizeof(char)*100*4);
            IR=R="";
            IC=0;
            C=false;
        }
        string get_mem(int pos)
        {
            string temp="";
            for(int i=0;i<4;i++)
                temp+=mem[pos][i];
            //cout<<"\n\ntemp of get_mem\t"<<temp;
            return temp;
        }
        void set_mem(string s, int pos)
        {


            for(int i=0;i<4;i++)
                mem[pos][i]=s[i];
        }
        void get_program_cards()
        {
            int flag=0;
            for(int i=0;i<100;i++)
            {
                for(int j=0;j<=3;j++)
                {
                    fin>>ch;
                  
                    mem[i][j]=ch;
                    if(mem[i][0]=='H')
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag)
                {
                //   fin>>ch;
                //   if(ch!='$')
                //   {
                //     for(int j=0;j<4;j++)
                //     {
                //         mem[i+1][j]=ch;
                //         fin>>ch;

                //     }

                //   }  
                    
                     
                break;}

            }
        }
        void set_IC()
        {
            IC=0;
        }
        void set_IC(int pos)
        {
            IC=pos;
            
        }
        int get_IC()
        {
            
            return (IC++);
        }  
        void set_IR(int IC)
        {
            IR="";
            for(int i=0;i<4;i++)
            {
                IR+=mem[IC][i];

            }
           
        }
        string get_IR()
        {
            return IR;
           // cout<<"\n\nir\t"<<IR;
        }
        void set_R(int pos)
        {
            R="";
            for(int i=0;i<4;i++)
                R+=mem[pos][i];

            //cout<<R<<"%"<<endl;    
        }
        string get_R()
        {
            return R;
        }
        void set_C(bool value)
        {
            C=value;
            //cout<<"Val of C "<<C<<endl;
        }
        bool get_C()
        {
            //cout<<"Val of C "<<C<<endl;
            return C;
        }
        void printmem()
		{
            for(int i=0;i<100;i++)
			{
                cout<<"Data at mem location"<<"["<<i<<"]";
                for(int j=0;j<4;j++)
                {
                    cout<<mem[i][j];
                }
                cout<<"\n";

			}
         
		}
}m_obj;

class cpu
{
    private:
        int fetched_IC;
        bool terminate,fetched_C;
        string fetched_IR,operand,opreator,fetched_R,compare_string;
    public:
       int s_to_i(string operand)
        {
            return ((int)operand[0]-48)*10+((int)operand[1]-48);
        }
        void startexe()
        {
            m_obj.set_IC();
            userprog();
        }      
        void userprog()
        {
            terminate=false;
            while(!terminate)
            {
                fetched_IC = m_obj.get_IC();
                m_obj.set_IR(fetched_IC);
                fetched_IR=m_obj.get_IR();
                opreator=fetched_IR.substr(0,2);
                operand=fetched_IR.substr(2,2);
                if(!(opreator.compare("LR")))
                {
                    int pos=s_to_i(operand);
                    m_obj.set_R(pos);
                }
                else if (!(opreator.compare("SR")))
                {
                    fetched_R=m_obj.get_R();
                    int pos=s_to_i(operand);
                    m_obj.set_mem(fetched_R, pos);
                }
                else if (!(opreator.compare("CR")))
                {
                    fetched_R=m_obj.get_R();
                    int pos=s_to_i(operand);
                    compare_string=m_obj.get_mem(pos);
                    if(fetched_R.compare(compare_string)==0)
                        m_obj.set_C(true);
                    else
                        m_obj.set_C(false);
                }
                else if (!(opreator.compare("BT")))
                {
                    fetched_C=m_obj.get_C();
                    if(fetched_C)
                    {
                        int pos=s_to_i(operand);
                        m_obj.set_IC(pos);
                    }
                }
                else if (!(opreator.compare("GD")))
                {
                    SI=1;
                    MOS();
                }
                else if (!(opreator.compare("PD")))
                {
                    SI=2;
                    MOS();
                }
                else
                {
                    SI=3;
                    MOS();
                }
            }
        }
        void MOS()
        {if(SI==1)
            {
                string s;
                int pos=s_to_i(operand);
                pos=(pos/10)*10;
                getline(fin,s);
    
                int len=s.length(),start=0,i;
                string s1;
                for(i=pos;start<len;i++)
                {
                    if((len-start)<4)
                        s1=s.substr(start,(len-start));
                    else    
                        s1=s.substr(start,4);
                    start+=4;
                    m_obj.set_mem(s1,i);
                }
            }
            else if(SI==2)
            {
                int pos=s_to_i(operand),flag=0;
                pos=(pos/10)*10;
                string ans="",temp="";
                for(int i=pos;i<pos+10;i++)
                {
                    temp=m_obj.get_mem(i);
                    for(int j=0;j<4;j++)
                    {
                        if(temp[j]=='\0' || temp[j]=='$')
                        {
                            break;
                            flag=1;
                        }
                        ans+=temp[j];
                    }
                    if(flag)
                        break;
                }
                fout<<ans<<endl;
            }
            else
            {
                terminate=true;
                fout<<endl<<endl;
            }

          
        }
}exe;

void load()
{
	string s,s1;
	
	while(!(fin.eof())){
        getline(fin,s);
        if(s.find("$AMJ")!=-1){
            
			m_obj.reset();
			m_obj.get_program_cards();
            
            continue;

        }
        else if(s.find("$DTA")!=-1)
            exe.startexe();
        else if(s.find("$END")!=-1)
            continue;

    }
    // string s,s1;
    // while(!(fin.eof()))
    // {
    //     getline(fin,s);
    //     if(s.empty())
    //         break;
    //     if(s.find("$AMJ")!=-1)
    //     {

    //         m_obj.reset();
    //         continue;
    //     }
    //     else if(s.find("$DTA")!=-1)
    //     {
    //         exe.startexe();
    //     }
    //     else if(s.find("$END")!=-1)
    //     {

    //         continue;
    //     }
    //     else
    //     {
    //         m_obj.get_program_cards();
    //     }
    // }
 
}





int main()
{
	fin.open("inputp.txt");
	fout.open("output.txt");

	
	load();
    m_obj.printmem();
    fin.close();
    fout.close();

	
    return 0;

}
