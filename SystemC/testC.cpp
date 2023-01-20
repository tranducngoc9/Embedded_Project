#include <iostream>
#include <systemc.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define MAX_CUSTOMER 5
#define MAX_CARD 5
#define ADULT_PRICE 20000
#define KID_PRICE 10000
#define ADULT_AGE 18
#define FIRST_CARD_ID 100

sc_event ePayment, eHandleBarieIn, eHandleBarieOut, eOut;

class Customer
{
private:
    int customerId;
    string name;
    int age;
    int cardId;

public:
    Customer(){};

    Customer(int customerId, string name, int age, int cardId)
    {
        this->customerId = customerId;
        this->name = name;
        this->age = age;
        this->cardId = cardId;
    };

    void setCustomerId(int customerId)
    {
        this->customerId = customerId;
    };

    void setName(string name)
    {
        this->name = name;
    };

    void setAge(int age)
    {
        this->age = age;
    };

    void setCardId(int cardId)
    {
        this->cardId = cardId;
    };

    int getCardId()
    {
        return this->cardId;
    };

    int getCustomerId()
    {
        return this->customerId;
    };

    string getName()
    {
        return this->name;
    };

    int getAge()
    {
        return this->age;
    };

    bool checkCustomerId(int customerId)
    {
        return this->customerId == customerId;
    };

    bool confirmCardId(int cardId)
    {
        return (this->cardId == cardId) ? true : false;
    };
};

class Card
{
private:
    int cardId;
    int balance;

public:
    Card(){};

    Card(int cardId, int balance)
    {
        this->cardId = cardId;
        this->balance = balance;
    };

    void setBalance(int balance)
    {
        this->balance = balance;
    };

    int getCardId()
    {
        return this->cardId;
    };

    int getBalance()
    {
        return this->balance;
    };

    void updateBalance(int money)
    {
        this->balance = this->balance - money;
    };
};

SC_MODULE(SwipeIn)
{
    sc_in<int> cardId;

    SC_CTOR(SwipeIn)
    {
        SC_THREAD(getInformationCustomer);
        sensitive << cardId;
    }

    void getInformationCustomer()
    {
        Customer customer[MAX_CUSTOMER];
        customer[0] = Customer(1, "Dang Truong", 18, 100);
        customer[1] = Customer(2, "Cong Anh", 18, 101);
        customer[2] = Customer(3, "Khoa Lee", 13, 102);
        customer[3] = Customer(4, "Duc Ngoc", 14, 103);
        customer[4] = Customer(5, "Hoa Thanh", 23, 63);

        Card card[MAX_CUSTOMER];
        card[0] = Card(100, 100000);
        card[1] = Card(101, 13000);
        card[2] = Card(102, 30000);
        card[3] = Card(103, 4000);
        card[4] = Card(104, 30000);

        int cardIdTemp = cardId;

        wait(1, SC_SEC);

        cout << "TRAM LEN TAU\n===========================================================================================" << endl;

        for (int n = 0; n < MAX_CUSTOMER; n++)
        {
            int customerId = 0;
            int customerIdTemp = 0;
            int ageTemp = 0;
            string nameTemp;

            for (int i = 0; i < MAX_CUSTOMER; i++)
            {
                if (customer[i].confirmCardId(cardIdTemp) == true)
                {
                    customerIdTemp = customer[i].getCustomerId();
                    nameTemp = customer[i].getName();
                    ageTemp = customer[i].getAge();
                }
            }

            if (customerIdTemp)
            {
                customerId = customerIdTemp;
                cout << "@" << sc_time_stamp() << "\tID khach hang: " << customerIdTemp
                     << "\tTen khach hang: " << nameTemp << "\t\tTuoi khach hang: " << ageTemp << endl;
            }
            else
            {
                cout << "@" << sc_time_stamp() << "\tKhong tim thay thong tin khach hang!" << endl;
            }

            ePayment.notify(1, SC_SEC);
            payment(customerId);
            cardIdTemp++;
        }

        eOut.notify(1, SC_SEC);

        cout << "===========================================================================================" << endl;
        cout << "TRAM XUONG TAU" << endl;
        cout << "===========================================================================================" << endl;
    }

    void payment(int customerId)
    {
        Customer customer[MAX_CUSTOMER];
        customer[0] = Customer(1, "Dang Truong", 18, 100);
        customer[1] = Customer(2, "Cong Anh", 18, 101);
        customer[2] = Customer(3, "Khoa Lee", 13, 102);
        customer[3] = Customer(4, "Duc Ngoc", 14, 103);
        customer[4] = Customer(5, "Hoa Thanh", 23, 63);

        Card card[MAX_CUSTOMER];
        card[0] = Card(100, 100000);
        card[1] = Card(101, 13000);
        card[2] = Card(102, 30000);
        card[3] = Card(103, 4000);
        card[4] = Card(104, 30000);

        bool statusPayment = false;
        wait(ePayment);
        bool id = false;

        for (int i = 0; i < MAX_CUSTOMER; i++)
        {
            int tempCardNumber = 0;

            if (customer[i].getCustomerId() == customerId)
            {
                id = true;

                for (int j = 0; j < MAX_CUSTOMER; j++)
                {
                    if (card[j].getCardId() == customer[i].getCardId())
                    {
                        tempCardNumber = j;
                        break;
                    }
                }

                if (customer[i].getAge() >= ADULT_AGE && card[tempCardNumber].getBalance() >= ADULT_PRICE)
                {
                    card[tempCardNumber].updateBalance(ADULT_PRICE);
                    statusPayment = true;
                    cout << "@" << sc_time_stamp() << "\tTru tien thanh cong!" << endl;
                }
                else if (customer[i].getAge() < ADULT_AGE && card[tempCardNumber].getBalance() >= KID_PRICE)
                {
                    statusPayment = true;
                    card[tempCardNumber].updateBalance(KID_PRICE);
                    cout << "@" << sc_time_stamp() << "\tTru tien thanh cong!" << endl;
                }
                else
                {
                    statusPayment = false;
                    cout << "@" << sc_time_stamp() << "\tTai khoan het tien!" << endl;
                }

                eHandleBarieIn.notify(1, SC_SEC);
            }
        }

        if (id == false)
        {
            statusPayment = false;
            eHandleBarieIn.notify(1, SC_SEC);
        }

        handleBarie(statusPayment);
    }

    void handleBarie(int statusPayment)
    {
        bool statusSensor = false;
        wait(eHandleBarieIn);

        if (statusPayment == true)
        {
            cout << "@" << sc_time_stamp() << "\tMo barie!" << endl;
            statusSensor = true;
            srand(time(NULL));
            int waitTime = (5 + rand() % 10);
            wait(waitTime, SC_SEC);
            statusSensor = false;
        }
        else
        {
            cout << "@" << sc_time_stamp() << "\tKhong mo barie!" << endl;
        }

        if (statusSensor == false)
        {
            cout << "@" << sc_time_stamp() << "\tDong barie!" << endl;
        }
    }
};

SC_MODULE(SwipeOut)
{
    sc_in<int> cardId;

    SC_CTOR(SwipeOut)
    {
        SC_THREAD(getInformationCustomer);
        sensitive << cardId;
    };

    void getInformationCustomer()
    {
        wait(eOut);

        int cardIdTemp = cardId;

        Customer customer[MAX_CUSTOMER];
        customer[0] = Customer(1, "Long Khanh", 18, 100);
        customer[1] = Customer(2, "Duc Thinh", 18, 101);
        customer[2] = Customer(3, "Manh Dung", 13, 102);
        customer[3] = Customer(4, "Hai Long", 14, 103);
        customer[4] = Customer(5, "Tuan Tu", 23, 63);

        Card card[MAX_CUSTOMER];
        card[0] = Card(100, 100000);
        card[1] = Card(101, 13000);
        card[2] = Card(102, 30000);
        card[3] = Card(103, 4000);
        card[4] = Card(104, 30000);

        wait(1, SC_SEC);

        for (int abc = 0; abc < 5; abc++)
        {
            int customerId = 0;
            int customerIdTemp = 0;
            int ageTemp = 0;
            bool isVisible = false;
            string nameTemp;

            for (int i = 0; i < MAX_CUSTOMER; i++)
            {
                if (customer[i].confirmCardId(cardIdTemp) == true)
                {
                    customerIdTemp = customer[i].getCustomerId();
                    nameTemp = customer[i].getName();
                    ageTemp = customer[i].getAge();
                }
            }

            if (customerIdTemp)
            {
                customerId = customerIdTemp;
                isVisible = true;
                cout << "@" << sc_time_stamp() << "\tID khach hang: " << customerIdTemp
                     << "\tTen khach hang: " << nameTemp << "\t\tTuoi khach hang: " << ageTemp << endl;
            }
            else
            {
                cout << "@" << sc_time_stamp() << "\tKhong tim thay thong tin khach hang!" << endl;
            }

            eHandleBarieOut.notify(1, SC_SEC);
            handleBarie(customerId);
            cardIdTemp++;
        }
        cout << "===========================================================================================" << endl;
    };

    void handleBarie(bool isVisible)
    {
        wait(eHandleBarieOut);

        bool statusSensor = false;

        if (isVisible == true)
        {
            cout << "@" << sc_time_stamp() << "\tMo barie!" << endl;
            statusSensor = true;
            srand(time(NULL));
            int waitTime = rand() % 10;
            wait(waitTime, SC_SEC);
            statusSensor = false;
        }
        else
        {
            cout << "@" << sc_time_stamp() << "\tKhong mo barie!" << endl;
        }

        if (statusSensor == false)
        {
            cout << "@" << sc_time_stamp() << "\tDong barie!" << endl;
        }
    }
};

SC_MODULE(HelloWorld)
{
    SC_CTOR(HelloWorld)
    {
        SC_THREAD(helloWorld);
    };

    void helloWorld()
    {
        cout << "\n\n\n\n";
        cout << "\t\t\t     ================================\n";
        cout << "\t\t\t         TRAM TAU DIEN BACH KHOA" << endl;
        cout << "\t\t\t     ================================\n";
    };
};

int sc_main(int argc, char *argv[])
{

    sc_signal<int> customerId;
    sc_signal<int> cardId;
    sc_signal<int> balance;
    sc_signal<int> age;
    sc_signal<bool> statusPayment;

    cardId = FIRST_CARD_ID;

    HelloWorld helloWorld("HelloWorld");

    SwipeIn swipeIn("SwipeIn");
    swipeIn.cardId(cardId);

    SwipeOut swipeOut("SwipeOut");
    swipeOut.cardId(cardId);

    sc_start(150, SC_SEC);
    return 0;
}