#include<iostream>
#include<string>
#include<string.h>
#include<fstream>
#include<iomanip>
using namespace std;

class bal
{
private:
  float balance;
public:
  bal();
  void write(float);
    void read(float);
  void check();
};


class admin
{
 private:
  char name[20];
  int pass;
 public:

};
class staff
{
public:
  char name[10];
  int pass;
public:
  staff();
  void write();
  void read();
};

staff::staff()
{
  strcpy(name,"");
  pass=0;
}
void staff::write()
{
  ofstream f("staff.dat");
  cout<<"Enter userid of Staff :";cin>>name;
  cout<<"Enter staff's password:";cin>>pass;
  f.write((char*)this,sizeof(staff));
  cout<<"\nSince Only one Staff is allowed,data was OVERWRITTEN.\n";

}
void staff::read()
{
  ifstream f("staff.dat");
  f.read((char*)this,sizeof(staff));

  cout<<"Userid of Staff :";cout<<name;
  cout<<"Staff's password:";cout<<pass;
  
}

void bal::check()
{
 ifstream fout("totalbal.dat");
 float bal=5;
  fout.read((char*)&bal,sizeof(bal));
  cout<<"\nTotal Profit is  :"<<bal<<"\n";
}

void bal::write(float x)
{
  fstream fout("totalbal.dat");
  float bal;
  fout.read((char*)&bal,sizeof(bal));
  bal+=x;
  fout.write((char*)&bal,sizeof(bal));
}

void bal::read(float x)
{
  fstream fout("totalbal.dat");
  float bal;
  fout.read((char*)&bal,sizeof(bal));
  bal-=x;
  fout.write((char*)&bal,sizeof(bal));
}


class purc;
class cust;
class grocery
{
protected:
	char rno[4];
	char iname[20];
    int quant;
	float cost;
	char status;
	int quantl;
public:
	grocery();
	void disp();
	void input();
	void write();
	void modify();
	void modifyrecord();
	void writeempty();
	void readf();
	int recno(char *);
	void idelete();
	void search();

	friend void pwrite(purc,grocery);
	friend void cwrite(cust,grocery);
	friend void searchbill(cust,grocery);

};
class purc:public grocery
{
protected:
	int sellid;
	char sellname[20];
	int bdate;
	float quantity;
public:
	purc();
	void disp();
	void input();
	friend void pwrite(purc,grocery);
	void modify();
	void modifyrecord();
	void writeempty();
	void readf();
	int recno(char *);
	void idelete();
	void search();
	int lastr();

};

int purc::lastr()
{
ifstream fin("purc.dat");
purc p;
fin.read((char*)&p,sizeof(purc));
int c=0;
while(fin.eof()==0)
{
fin.read((char*)&p,sizeof(purc));
c++;
}
return c;
}

class cust:public grocery
{
protected:
	int bid;
	int custid;
	char custname[20];
	int cdate;
public:
	cust();
	void disp();
	void input();
	void modify();
	void modifyrecord();
	void writeempty();
	void readf();
	int recno(char *);
	void idelete();
	void search();
	int lastr();
	friend void cwrite(cust,grocery);
	friend void searchbill(cust,grocery);
	int lastbid();


};

int cust::lastbid()
{

ifstream fin("cust.dat");
cust p;
fin.read((char*)&p,sizeof(cust));
int c=0;
while(fin.eof()==0)
{
fin.read((char*)&p,sizeof(cust));
c++;
}
fin.seekg((c-1)*sizeof(cust));
fin.read((char*)&p,sizeof(cust));

return p.bid;

}

int cust::lastr()
{
ifstream fin("cust.dat");
cust p;
fin.read((char*)&p,sizeof(cust));
int c=0;
while(fin.eof()==0)
{
c++;
fin.read((char*)&p,sizeof(cust));
}
return c;
}



cust::cust():grocery()
{
	custid=0;
	strcpy(custname," ");
	cdate=0;
	cost=0;
	quant=0;
	bid=0;
}


purc::purc():grocery()
{
sellid=0;
strcpy(sellname," ");
bdate=0;
cost=0;
quant=0;
}

/*
void purc::writeempty()
{
	ofstream fout("purc.dat");

for(int i=0;i<300;i++)
fout.write((char *)this,sizeof(purc));
}
*/


void cwrite(cust c,grocery g)
{
	fstream fout("cust.dat");
	fstream fout2("stock2.dat");
	c.bid=c.lastbid()+1;
	
	cout<<"Bill No."<<c.bid;
//	cout<<"\n"<<c.lastr()<<"\n";
	cin.ignore();	cout<<"\nCustomer Name    : ";cin.getline(c.custname,20);
	cout<<"Customer ID     : ";cin>>c.custid;
	cout<<"Purchase Date       : ";cin>>c.cdate;
	int bopt=1;
	fout.seekp((c.lastr())*sizeof(cust));
	while(bopt==1)
	{
		cout<<"\nRecord Number: ";cin>>c.rno;
		fout2.seekg((g.recno(c.rno))*sizeof(grocery),ios::beg);
		fout2.read((char*)&g,sizeof(grocery));
		if(strcmp(g.rno,"000"))   //if equal 0,else 1
			{
			c.modifyrecord();
			if(c.quant<=g.quant)
				{
				strcpy(c.rno,g.rno);
				strcpy(c.iname,g.iname);
				c.cost=g.cost;
				fout.write((char*)&c,sizeof(cust));
				g.quant-=c.quant;
				if(g.quant<g.quantl)
				g.status='u';
				fout2.seekp(g.recno(g.rno)*sizeof(grocery),ios::beg);
				fout2.write((char*)&g,sizeof(grocery));
				}
			else
				{
				cout<<"\nThe quantity required is not there..\n";
				}
		}
	else
	{
	cout<<"\nRecord is missing .Cannot bill\n";
	strcpy(g.rno,c.rno);
	}
	
	cout<<"Enter 1-Continue Billing.\n";
	cin>>bopt;
	}

	cout<<"_________________________________________________________________________\n\t\t\tXYZ GROCERY SHOP\n_________________________________________________________________________\n";
	cout<<"Bill Number :"<<setw(4)<<setfill('0')<<c.bid<<setfill(' ')<<"\tCustomer ID :"<<c.custid<<"\tCustomer Name:"<<c.custname<<"\n"<<"Date of Purchase :"<<c.cdate<<"\n";
cout<<"_______________________________________________________________________\n";
fout.seekg(0);
cust c1;
fout.read((char*)&c1,sizeof(cust));
int i=1,sum=0;
cout<<"\nS.No.     ItemCode    Item Name     Cost(per)    Quantity    Cost\n-------------------------------------------------------------------------\n";
while(fout.eof()==0)
{
	if(c1.bid==c.bid)
	{
		sum+=c1.quant*c1.cost;
		cout<<setw(5)<<i<<setw(10)<<c1.rno<<setw(15)<<c1.iname<<setw(15)<<c1.cost<<setw(10)<<c1.quant<<setw(10)<<c1.quant*c1.cost<<"\n";
	i++;
	}
	fout.read((char*)&c1,sizeof(cust));
}
cout<<"\n-------------------------------------------------------------------------\n";
cout<<"Total Cost :\t\t\t\t\t\t\t"<<sum;
cout<<"\n-------------------------------------------------------------------------\n";
cout<<"Thank You for purchasing from XYZ Grocery Shop. Welcome again !!!!!!\n";
}


void searchbill(cust c1,grocery g)
{
	fstream fout("cust.dat");
	cout<<"\nEnter Bill Number to be searched : ";
	int bno;
	cin>>bno;


fout.seekg(0);
fout.read((char*)&c1,sizeof(cust));
int i=1,sum=0;
while(fout.eof()==0)
{
	if(c1.bid==bno)
	{
		if(i==1)
		{
		cout<<"_________________________________________________________________________\n\t\t\tXYZ GROCERY SHOP\n_________________________________________________________________________\n";
		cout<<"Bill Number :"<<setw(4)<<setfill('0')<<c1.bid<<setfill(' ')<<"\tCustomer ID :"<<c1.custid<<"\tCustomer Name:"<<c1.custname<<"\n"<<"Date of Purchase :"<<c1.cdate<<"\n";
		cout<<"_______________________________________________________________________\n";
		cout<<"\nS.No.     ItemCode    Item Name     Cost(per)    Quantity    Cost\n-------------------------------------------------------------------------\n";

		}
		sum+=c1.quant*c1.cost;
		cout<<setw(5)<<i<<setw(10)<<c1.rno<<setw(15)<<c1.iname<<setw(15)<<c1.cost<<setw(10)<<c1.quant<<setw(10)<<c1.quant*c1.cost<<"\n";
	i++;
	}
fout.read((char*)&c1,sizeof(cust));
}
if(i!=1)
{
cout<<"\n-------------------------------------------------------------------------\n";
cout<<"Total Cost :\t\t\t\t\t\t\t"<<sum;
cout<<"\n-------------------------------------------------------------------------\n";
}
if(i==1)
{
	cout<<"\nSorry Bill doesn't exist.\n";
}


}

void pwrite(purc p,grocery g)
{
        fstream fout("purc.dat");
	fstream fout2("stock2.dat");
	
	cout<<"\nPurchase Order # "<<setw(5)<<setfill('0')<<p.lastr()+1<<"\n"<<setfill(' ');;
	
	cout<<"\nRecord Number: ";cin>>p.rno;
	fout2.seekg((g.recno(p.rno))*sizeof(grocery),ios::beg);
	fout2.read((char*)&g,sizeof(grocery));
	
	if(strcmp(g.rno,"000"))   
	{
	p.modifyrecord();
	strcpy(p.rno,g.rno);
	strcpy(p.iname,g.iname);
	}
	else
	{
	cout<<"\nData is absent.Add this a new record to stock database.";
	strcpy(g.rno,p.rno);

	g.modifyrecord();
	if(g.cost>0&&g.quant>=0)
	{
	strcpy(p.rno,g.rno);
	strcpy(p.iname,g.iname);
	p.modifyrecord();
	}
	else
	{
		cout<<"\nStock Quantity or/and Cost is(are) incorrect.Record NOT added.\n";
	}
	}
	if(p.quant>0&&p.cost>0&&g.cost>0&&g.quant>=0)
	{
	fout.seekp(p.lastr()*sizeof(purc));
	fout.write((char *)&p,sizeof(purc));
	float x=p.cost*p.quant;
//totalbaladd(x);
	g.quant+=p.quant;

	fout2.seekp(g.recno(g.rno)*sizeof(grocery),ios::beg);
	fout2.write((char*)&g,sizeof(grocery));
	}
	else
	{
		cout<<"\nPurchase Quantity or/and Cost is(are) incorrect.Record NOT added.\n"; 
	}
		fout.close();
	
}

grocery::grocery()
{
	strcpy(rno,"000");
	strcpy(iname,"00");

	quant=0;
	cost=0;
	status='i';
	quantl=0;
}


void grocery::disp()
{
	cout<<"\n"<<left<<"  "<<setw(15)<<rno<<setw(20)<<iname<<setw(10)<<quant<<setw(10)<<cost;
	if(status=='a'||status=='A')
		cout<<setw(10)<<"Active";
	if(status=='u'||status=='U')
		cout<<setw(10)<<"Inactive";

}

void purc::disp()
{
cout<<"\n"<<rno<<"\t\t"<<iname<<"\t"<<sellname<<"\t"<<sellid<<"\t"<<quant<<"\t\t"<<cost<<"\t"<<status<<"\t"<<bdate;
}

void cust::disp()
{
cout<<"\n   "<<bid<<"\t"<<rno<<"\t"<<iname<<"\t\t"<<custname<<"\t "<<custid<<"\t   "<<quant<<"\t"<<cost<<"\t"<<cdate;
}

void grocery::input()
{
	cout<<"\nRecord Number           : ";cin>>rno;
	cin.ignore();
	cout<<"Item Name               : ";cin>>iname;
	cout<<"Quantity(kg/lt/pieces)  : ";cin>>quant;
	cout<<"Cost    (kg/lt/pieces)  : ";cin>>cost;
	cout<<"Quantity Limit          : ";cin>>quantl;
	if(quantl<=quant)
	status='a';
	else
	status='u';

}
void grocery::modifyrecord()
{
cin.ignore();
	cout<<"\nItem Name             : ";cin.getline(iname,20);
	cout<<"Quantity(kg/lt/pieces): ";cin>>quant;
	cout<<"Cost(kg/lt/pieces)    : ";cin>>cost;
	cout<<"Quantity Limit          : ";cin>>quantl;
	if(quantl<=quant)
status='a';
else
status='u';
}

void purc::modifyrecord()
{
cin.ignore();	cout<<"\nSeller Name    : ";cin.getline(sellname,20);
	cout<<"Seller ID     : ";cin>>sellid;
	cout<<"Bought Date         : ";cin>>bdate;
	cout<<"Quantity bought   :";cin>>quant;
	cout<<"Cost Price       :";cin>>cost;
}

void cust::modifyrecord()
{
//cin.ignore();	cout<<"\nCustomer Name    : ";cin.getline(custname,20);
	//cout<<"Customer ID     : ";cin>>custid;
	//cout<<"Purchase Date       : ";cin>>cdate;
	cout<<"Quantity buying   :";cin>>quant;
	//cout<<"Cost Price       :";cin>>cost;
	

}


int  grocery::recno(char *ptr)
{
	int c=ptr[0],first=ptr[1],second=ptr[2];
	c=tolower(c);
	int sum=((c-97)*100)+((first-48)*10)+(second-48);
	return sum;

}
void grocery::writeempty()
{
ofstream fout("stock2.dat");
for(int i=0;i<2600;i++)
fout.write((char *)this,sizeof(grocery));
}

void grocery::write()
{
	fstream fout("stock2.dat");
	ifstream fin("stock2.dat");
	fout.seekp(0);
	grocery temp;
	cout<<"\nRecord Number: ";cin>>rno;
	fin.seekg(recno(rno)*sizeof(grocery),ios::beg);
	fin.read((char*)&temp,sizeof(grocery));
	if(!strcmp(temp.rno,"000"))
	{
	strcpy(temp.rno,rno);
	temp.modifyrecord();
	if(temp.quant>0&&temp.cost>0)
	{
	fout.seekp(recno(temp.rno)*sizeof(grocery),ios::beg);
	fout.write((char*)&temp,sizeof(grocery));
	}
	else
	{
	cout<<"Quantity or/and Cost entered incorrectly.Record NOT added.\n";
}
	}
	else
	cout<<"\nData is already present, instead try modifying.";
	
	fout.close();
}
void grocery::readf()
{
	int count=0;
	ifstream fin("stock2.dat");
	if(!fin)
	{
	cout<<"File could not be copened.";
	writeempty();
	cout<<"File writtten for FIRST TIME.\n";
	}
	grocery temp;
	cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";
	fin.read((char *)&temp,sizeof(grocery));

	while(fin.eof()==0)
	{
		if(strcmp(temp.rno,"000"))
		{
		temp.disp();
		count++;
		}
		fin.read((char *)&temp,sizeof(grocery));
	}
	cout<<"\n--> "<<count<<" record(s) found.\n";

ifstream fin1("stock2.dat");
fin1.seekg(0);
int count1=0;
fin1.read((char*)&temp,sizeof(grocery));
while(fin1.eof()==0)
{

	if(temp.status=='u'||temp.status=='U')
{
	if(count1==0)
	cout<<"The Quantity of following records are insufficient and status are set to INACTIVE ,please purchase.\n";
		temp.disp();
	count1++;
}
	fin1.read((char*)&temp,sizeof(grocery));
}


	fin.close();
	
}

void purc::readf()
{
	int count=0;
	ifstream fin("purc.dat");
	if(!fin)
	{
	cout<<"File could not be copened.";
	//writeempty();
	cout<<"File writtten for FIRST TIME.\n";
	}
	purc temp;
	cout<<"Record No.\tItem name\tQuantity\tCost\tStatus";
	
	fin.read((char *)&temp,sizeof(purc));

	while(fin.eof()==0)
	{
		temp.disp();
		count++;
		fin.read((char *)&temp,sizeof(purc));
	}
	cout<<"\n--> "<<count<<" record(s) found.\n";
	fin.close();
	
}


void cust::readf()
{
	int count=0;
	ifstream fin("cust.dat");
	if(!fin)
	{
	cout<<"File could not be copened.";
	//writeempty();
	cout<<"File writtten for FIRST TIME.\n";
	}
	cust temp;
	cout<<"BillNo. RNo.    Item name       Cust.Name  CustID    Quantity   Cost    Status\n";
	
	fin.read((char *)&temp,sizeof(cust));

	while(fin.eof()==0)
	{
		temp.disp();
		count++;
		fin.read((char *)&temp,sizeof(cust));
	}
	cout<<"\n--> "<<count<<" record(s) found.\n";
	fin.close();
	
}







void grocery::idelete()
{
fstream fout("stock2.dat");
ifstream fin("stock2.dat");
fout.seekp(0);
grocery temp;
cout<<"\nEnter the record to be deleted:  ";
cin>>this->rno;
fin.seekg(recno(this->rno)*sizeof(grocery),ios::beg);
fin.read((char *)this,sizeof(grocery));
if(strcmp(this->rno,"000"))
{
	fout.seekp(recno(this->rno)*sizeof(grocery),ios::beg);
	fout.write((char *)&temp,sizeof(grocery));
}
else
{
	cout<<"\nThe record no. doesn't exist.";
}

	fout.close();
}

void grocery::modify()
{
	grocery old;
fstream fout("stock2.dat");
ifstream fin("stock2.dat");
fout.seekp(0);
cout<<"\nEnter the record No. to be edited:  ";
cin>>this->rno;
fin.seekg(recno(this->rno)*sizeof(grocery),ios::beg);
fin.read((char *)&old,sizeof(grocery));
if(strcmp(old.rno,"000"))
{
this->modifyrecord();
if(this->quant>0&&this->cost>0)
{
fout.seekp(recno(this->rno)*sizeof(grocery),ios::beg);
fout.write((char *)this,sizeof(grocery));
}
else
{
	cout<<"Quantity or/and Cost entered incorrectly.Record NOT modified.\n";
}

}
else
{
	cout<<"\nData doesn't exist.";
}
fin.close();
fout.close();

}
void grocery::search()
{
	int count=0;
	ifstream fin("stock2.dat");
	if(!fin)
	cout<<"File could not be copened.";
	grocery temp;
	grocery search;
	char c;
	cin.get(c);
	cout<<"Search Criteria :1-Record No. 2-Item Name  3-Quantity   4-Cost  5-Status\nEnter option : ";
		cin.get(c);
		switch(c)
		{

			case '\n':
			case '1':
			cout<<"Enter Record No. to be searched: ";
			cin>>search.rno;
			fin.read((char *)&temp,sizeof(grocery));
			while(fin.eof()==0)
			{
				if(recno(temp.rno)==recno(search.rno))
					{
					if(count==0)
					cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";
					count++;
					temp.disp();
					}
					fin.read((char *)&temp,sizeof(grocery));
			}
			if(count==0)
			cout<<"\nSorry,Data doesn't exist.";
			break;

		
			case '2':
			cout<<"Enter Item Name. to be searched: ";
			cin>>search.iname;
			fin.read((char *)&temp,sizeof(grocery));
			while(fin.eof()==0)
				{
				if(!strcmp(temp.iname,search.iname))
					{
						if(count==0){
					cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";}
					count++;
					temp.disp();
					}
					fin.read((char *)&temp,sizeof(grocery));
				}
				if(count==0)
				cout<<"\nSorry,Data doesn't exist.";
				break;


case '3':

cout<<"Enter Quantity of item to be searched: ";
	cin>>search.quant;
	fin.read((char *)&temp,sizeof(grocery));
	while(fin.eof()==0)
	{
		if(temp.quant==search.quant)
		{
			if(count==0){
			cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";}
		count++;
		temp.disp();
		}
		fin.read((char *)&temp,sizeof(grocery));
	}
	if(count==0)
	cout<<"\nSorry,Data doesn't exist.";
	break;
case '4':

cout<<"Enter Cost of item to be searched: ";
	cin>>search.cost;
	fin.read((char *)&temp,sizeof(grocery));
	while(fin.eof()==0)
	{
		if(temp.cost==search.cost)
		{
			if(count==0){
				cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";}
		count++;
		temp.disp();
		}
		fin.read((char *)&temp,sizeof(grocery));
	}
	if(count==0)
	cout<<"\nSorry,Data doesn't exist.";
	break;

	case '5':

    cout<<"Enter Status of item A/U to be searched: ";
	cin>>search.status;
	fin.read((char *)&temp,sizeof(grocery));
	while(fin.eof()==0)
	{
		if(temp.status==search.status)
		{
		if(count==0)
		{
		cout<<"\n\n-------------------------------------------------------------------------------\n";
	cout<<"Record No.\tItem name\t  Quantity    Cost\t  Status";
	cout<<"\n-------------------------------------------------------------------------------\n";
		}
			count++;
		temp.disp();
		}
		fin.read((char *)&temp,sizeof(grocery));
	}
	if(count==0)
	cout<<"\nSorry,Data doesn't exist.";
	fin.close();
	
}
cout<<"\n";
}



int main()
{
	cust c;
	purc p;
	grocery g;
	float bal=0;
	//ofstream fout("totalbal.dat");
	//fout.write((char*)&bal,sizeof(float));





  	cout<<"----------------------------------------------------\n\tGROCERY MANAGEMENT SYSTEM\n----------------------------------------------------";
	cout<<"\nADMINISTRATOR(A) or STAFF(S)   :";
	char ch;
	cin>>ch;
	char uname[10];
	int pass;
	if(ch=='a'||ch=='A')
	{
		cout<<"\nUsername     :";
		cin>>uname;
		cout<<"Password     :";
		cin>>pass;
		
		if(!strcmp(uname,"admin")&&pass==1234)
		{
			cout<<"\nAccess Granted.\n";
		
	
	cout<<"\n----------------\nMAIN MENU\n----------------\n";
	cout<<"1-BILLING  2-STOCK PURCHASE  3-STOCK DATA  4-OPTIONS  9-CLEAR  0-EXIT.\nEnter option: ";
	int x;
	cin>>x;
	while(x!=0)
	{
		switch(x)
		{
		case 1:
			cout<<"\n----------------\nBILLING\n----------------\n";
			cout<<"1-START A BILL  2-DISPLAY ALL  3-SEARCH BILL  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
			int y;
			cin>>y;
			while(y!=0)
			{
				switch(y)
				{
				case 1:
					cwrite(c,g);
					break;
				case 2:
					c.readf();
					break;
				case 3:
					searchbill(c,g);
					break;
				default:
					cout<<"\nInvalid Option.";
				}
				cout<<"\n----------------\nBILLING\n----------------\n";
				cout<<"1-START A BILL  2-DISPLAY ALL  3-SEARCH BILL  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				cin>>y;
			}
			break;


		        case 2:
			cout<<"\n----------------\nSTOCK PURCHASE\n----------------\n";
			cout<<"1-PURCHASE STOCK  2-DISPLAY ALL 3-SEARCH PURCHASE ORDER  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
			cin>>y;
			while(y!=0)
			{
				switch(y)
				{
				case 1:
					pwrite(p,g);
					break;
				case 2:
					p.readf();
					break;
				default:
					cout<<"\nInvalid Option.";
				}
				cout<<"\n----------------\nSTOCK PURCHASE\n----------------\n";
				cout<<"1-PURCHASE STOCK  2-DISPLAY ALL 3-SEARCH PURCHASE ORDER  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				cin>>y;
			}
			break;
		case 3:
			cout<<"\n----------------\nSTOCK DATA\n----------------\n";
			cout<<"1-READ FILE  2-ADD  3-DELETE  4-MODIFY  5-SEARCH  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
			cin>>y;
			while(y!=0)
			{
				switch(y)
				{
				case 1:
					g.readf();
					break;
				case 2:
					g.write();
					break;
				case 3:
					g.idelete();
					break;
				case 4:
					g.modify();
					break;
				case 5:
					g.search();
					break;
				default:
					cout<<"\nInvalid Option.";
				}
				cout<<"\n----------------\nSTOCK DATA\n----------------\n";
				cout<<"1-READ FILE  2-ADD  3-DELETE  4-MODIFY  5-SEARCH  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				cin>>y;
			}
			break;

			case 4:
			  cout<<"\n----------------\nOPTIONS\n----------------\n";
				cout<<"1-CHANGE STAFF 2-CHANGE PASSWORD  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				int x;
				cin>>x;
				while(x!=0)
				  {
				    switch(x)
				      {
				      case 1:
					staff s;
					s.write();
					break;
				      }
				    cout<<"\n----------------\nOPTIONS\n----------------\n";
				cout<<"1-CHANGE STAFF 2-CHANGE PASSWORD  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				    cin>>x;
				  }
				break;

		case 5:
//		  bal b;
//		  b.check();
		  break;


		case 6:
		  staff s;
		  s.write();
		  s.read();
		  break;

		}
		cout<<"\n----------------\nMAIN MENU\n----------------\n";
		cout<<"1-BILLING  2-STOCK PURCHASE  3-STOCK DATA  4-OPTIONS  9-CLEAR  0-EXIT.\nEnter option: ";
cin>>x;
	}
	}
	}
	else if(ch=='s'||ch=='S')
	{
		staff s;
		char uname[10];
		int pass;
		fstream f("staff.dat");
		cout<<"Username   :";
		cin>>uname;
		cout<<"Password   :";
		cin>>pass;
f.read((char*)&s,sizeof(staff));
if(strcpy(uname,s.name)&&s.pass==pass)
{
	

	cout<<"\nAccess Granted.\n";
		
	
	cout<<"\n----------------\nMAIN MENU\n----------------\n";
	cout<<"1-BILLING  2-STOCK DATA  9-CLEAR  0-EXIT.\nEnter option: ";
	int x;
	cin>>x;
	while(x!=0)
	{
		switch(x)
		{
		case 1:
			cout<<"\n----------------\nBILLING\n----------------\n";
			cout<<"1-START A BILL  2-DISPLAY ALL  3-SEARCH BILL  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
			int y;
			cin>>y;
			while(y!=0)
			{
				switch(y)
				{
				case 1:
					cwrite(c,g);
					break;
				case 2:
					c.readf();
					break;
				case 3:
					searchbill(c,g);
					break;
				default:
					cout<<"\nInvalid Option.";
				}
				cout<<"\n----------------\nBILLING\n----------------\n";
				cout<<"1-START A BILL  2-DISPLAY ALL  3-SEARCH BILL  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				cin>>y;
			}
			break;


		 	case 2:
			cout<<"\n----------------\nSTOCK DATA\n----------------\n";
			cout<<"1-READ FILE  2-SEARCH  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
			cin>>y;
			while(y!=0)
			{
				switch(y)
				{
				case 1:
					g.readf();
					break;
				case 2:
					g.search();
					break;
				default:
					cout<<"\nInvalid Option.";
				}
				cout<<"\n----------------\nSTOCK DATA\n----------------\n";
				cout<<"1-READ FILE  2-SEARCH  9-CLEAR  0-BACK TO MAIN MENU.\nEnter option:  ";
				cin>>y;
			}
			break;

			

		case 5:
//		  bal b;
//		  b.check();
		  break;


		case 6:
		  staff s;
		  s.write();
		  s.read();
		  break;

		}
		cout<<"\n----------------\nMAIN MENU\n----------------\n";
		cout<<"1-BILLING  2-STOCK DATA  9-CLEAR  0-EXIT.\nEnter option: ";
cin>>x;
	
	}



}	
else
	cout<<"\nWrong Option\nExiting............\n\n";
		
	}
	else
		cout<<"\nWrong Option\nExiting............\n\n";
	return 0;
}

