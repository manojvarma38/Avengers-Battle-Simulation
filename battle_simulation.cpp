#include <bits/stdc++.h>
using namespace std;
class QNS
{
private:
    int powerLevel, durability, energyStorage, heatLevel; // Attributes of suit

public:
    // Parameterized constructor with default arguments
    QNS(int powerLevel = 1000, int durability = 500, int energyStorage = 300, int heatLevel = 0)
    {
        this->powerLevel = min(5000, powerLevel);
        this->durability = durability;
        this->energyStorage = energyStorage;
        this->heatLevel = heatLevel;
    }
    // Copy constructor
    QNS(const QNS &otherSuit)
    {
        this->powerLevel = otherSuit.powerLevel;
        this->durability = otherSuit.durability;
        this->energyStorage = otherSuit.energyStorage;
        this->heatLevel = otherSuit.heatLevel;
    }
    // operator overloading for operators + - * / < ==
    void operator+(const QNS &otherSuit)
    {
        this->powerLevel = this->powerLevel + otherSuit.energyStorage;
        this->durability = this->durability + otherSuit.durability;
        this->energyStorage = this->energyStorage + otherSuit.powerLevel;
        if (this->powerLevel > 5000)
        {
            this->powerLevel = 5000;
        }
    }
    void operator-(int x)
    {
        this->durability -= x;
        this->energyStorage += x;
        this->heatLevel += x;
    }
    void operator*(int x)
    {
        this->powerLevel += ((this->powerLevel * x) / 100);
        this->energyStorage = this->energyStorage + 5 * x;
        this->heatLevel = this->heatLevel + x;
        if (this->powerLevel > 5000)
        {
            this->powerLevel = 5000; // max powerLevel
        }
    }
    void operator/(int x)
    {
        this->durability = this->durability + x;
        this->heatLevel = this->heatLevel - x;
        if (this->heatLevel < 0)
        {
            this->heatLevel = 0; // min heat level
        }
    }
    void boostPower(int factor)
    {
        (*this) * factor;
    }
    void boostPower(const QNS &otherSuit)
    {
        (*this) + otherSuit;
    }
    bool operator==(const QNS &otherSuit)
    {
        if (this->powerLevel == otherSuit.powerLevel && this->durability == otherSuit.durability)
            return true;
        return false;
    }
    bool operator<(const QNS &otherSuit)
    {
        if (this->powerLevel + this->durability < otherSuit.powerLevel + otherSuit.durability)
            return true;
        return false;
    }
    // Getter and Setter functions
    int getpowerLevel()
    {
        return this->powerLevel;
    }
    void setpowerLevel(int powerLevel)
    {
        this->powerLevel = powerLevel;
    }
    int getdurability()
    {
        return this->durability;
    }
    void setdurability(int durability)
    {
        this->durability = durability;
    }
    int getenergyStorage()
    {
        return this->energyStorage;
    }
    void setenergyStorage(int energyStorage)
    {
        this->energyStorage = energyStorage;
    }
    int getheatLevel()
    {
        return this->heatLevel;
    }
    void setheatLevel(int heatLevel)
    {
        this->heatLevel = heatLevel;
    }
};
// Global queue of suits to get the functionality of first come first serve ordering
queue<QNS> suits;
vector<string> battleLog;
class Avenger
{
private:
    string name;
    int attackStrength;
    QNS suit;

public:
    Avenger() {}
    Avenger(string avName, QNS avSuit, int strength) // Parameterized constructor
    {
        this->name = avName;
        this->suit = avSuit;
        this->attackStrength = strength;
    }
    void attack(Avenger &enemy)
    {
        // use references to not make copies
        Avenger &av1 = *this;
        Avenger &av2 = enemy;
        if (av1.suit.getdurability() > 0 && av1.suit.getheatLevel() <= 500)
        {
            if (av2.suit.getdurability() > 0)
            {
                // attack is possible
                string command = "";
                command += av1.name;
                command += " attacks ";
                command += av2.name;
                battleLog.push_back(command);
                av2.suit - av1.attackStrength;
                if (av2.suit.getdurability() <= 0)
                {
                    // durability check
                    string command = "";
                    command += av2.name;
                    command += " suit destroyed";
                    battleLog.push_back(command);
                }
                else if (av2.suit.getheatLevel() > 500)
                {
                    // heatlevel check
                    string command = "";
                    command += av2.name;
                    command += " suit overheated";
                    battleLog.push_back(command);
                }
            }
        }
    }
    void upgradeSuit()
    {
        if (suits.size() == 0)
        {
            // no suits are left
            string command = "";
            command += this->name;
            command += " upgrade Fail";
            battleLog.push_back(command);
            return;
        }
        // extra suits are left
        QNS newSuit = suits.front();
        suits.pop(); // pop the suit used
        this->suit + newSuit;
        string command = "";
        command += this->name;
        command += " upgraded";
        battleLog.push_back(command);
        return;
    }
    void repair(int x)
    {
        this->suit / x;
        string command = "";
        command += this->name;
        command += " repaired";
        battleLog.push_back(command);
        return;
    }
    void printStatus()
    {
        // print the status of avenger
        cout << this->name << ' ' << suit.getpowerLevel() << ' ' << suit.getdurability() << ' ' << suit.getenergyStorage() << ' ' << suit.getheatLevel() << '\n';
    }
    string getName()
    {
        return this->name;
    }
    QNS &getSuit()
    {
        return this->suit; // get the reference to suit
    }
};
class Battle
{

public:
    vector<Avenger> heroes, enemies;
    void printBattleLog()
    {
        for (auto it : battleLog)
        {
            cout << it << '\n';
        }
    }
    Avenger &getAvenger(string name) // get avenger from name
    {
        for (auto &it : heroes)
        {
            if (it.getName() == name)
            {
                return it;
            }
        }
        for (auto &it : enemies)
        {
            if (it.getName() == name)
            {
                return it;
            }
        }
        static Avenger dummy; // code never reaches here
        return dummy;
    }
    int Result()
    {
        // calculate result
        int sumHeroes = 0, sumEnemies = 0;
        for (auto &it : heroes)
        {
            QNS &suit = it.getSuit();
            if (suit.getdurability() > 0)
            {
                sumHeroes += suit.getdurability();
                sumHeroes += suit.getpowerLevel();
            }
        }
        for (auto &it : enemies)
        {
            QNS &suit = it.getSuit();
            if (suit.getdurability() > 0)
            {
                sumEnemies += suit.getdurability();
                sumEnemies += suit.getpowerLevel();
            }
        }
        if (sumHeroes > sumEnemies)
            return 1;
        else if (sumHeroes == sumEnemies)
            return 0;
        return -1;
    }
    void startBattle()
    {
        int k, n, m;
        cin >> k >> n >> m;
        for (int i = 0; i < k; i++)
        {
            int P, D, E, H;
            cin >> P >> D >> E >> H;
            QNS newSuit(P, D, E, H); // create suits using Parameterized constructor
            suits.push(newSuit);
        }
        for (int i = 0; i < n + m; i++)
        {
            string name;
            int attackStrength;
            cin >> name >> attackStrength;
            if (suits.size() == 0) // If suits are not available,Avenger is out of fight
            {
                cout << name << " is out of fight" << '\n';
            }
            else
            {
                // use queue to asign the suit to avenger and pop the suit from queue as it is used
                QNS newSuit = suits.front();
                suits.pop();
                Avenger newAvenger(name, newSuit, attackStrength);
                if (i < n) // first n avengers are heroes
                {
                    heroes.push_back(newAvenger);
                }
                else // last m avengers are enemies
                {
                    enemies.push_back(newAvenger);
                }
            }
        }
        string flag;
        cin >> flag;
        while (flag != "End") // keep taking the input until flag is not "End"
        {
            cin >> flag;
            if (flag == "Attack")
            {
                string avName1, avName2;
                cin >> avName1 >> avName2; // call attack function
                getAvenger(avName1).attack(getAvenger(avName2));
            }
            else if (flag == "Repair")
            {
                string avName;
                int x;
                cin >> avName >> x; // call repair function
                getAvenger(avName).repair(x);
            }
            else if (flag == "BoostPowerByFactor")
            {
                string avName;
                int y;
                cin >> avName >> y; // boost the avengers suit
                Avenger &av = getAvenger(avName);
                av.getSuit().boostPower(y);
                string command = "";
                command += avName;
                command += " boosted";
                battleLog.push_back(command);
                if (av.getSuit().getheatLevel() > 500)
                {
                    // heat level check
                    string command = "";
                    command += avName;
                    command += " suit overheated";
                    battleLog.push_back(command);
                }
            }
            else if (flag == "BoostPower")
            {
                string avName;
                int P, D, E, H;
                cin >> avName >> P >> D >> E >> H; // transfer P,D,E,H to avengers suit
                QNS newSuit = QNS(P, D, E, H);
                Avenger &av = getAvenger(avName);
                av.getSuit().boostPower(newSuit);
                string command = "";
                command += avName;
                command += " boosted";
                battleLog.push_back(command);
                if (av.getSuit().getheatLevel() > 500)
                {
                    string command = "";
                    command += avName;
                    command += " suit overheated";
                    battleLog.push_back(command);
                }
            }
            else if (flag == "AvengerStatus")
            {
                string avName;
                cin >> avName; // call printStatus function
                Avenger &av = getAvenger(avName);
                av.printStatus();
            }
            else if (flag == "Upgrade")
            {
                string avName;
                cin >> avName; // use queue to get the suit to upgrade current suit of avenger
                Avenger &av = getAvenger(avName);
                av.upgradeSuit();
            }
            else if (flag == "PrintBattleLog")
            {
                printBattleLog();
                // output the battlelog vector
            }
            else if (flag == "BattleStatus")
            {
                int res = Result();
                if (res == 1)
                    cout << "heroes are winning" << '\n';
                else if (res == 0)
                    cout << "tie" << '\n';
                else
                    cout << "enemies are winning" << '\n';
                // print status according to battle result
            }
            else if (flag == "End")
            {
                break;
            }
        }
    }
};
int main()
{
    Battle battle;
    battle.startBattle(); // start to take the input

    return 0;
}