/* 
 * File:   main.cpp
 * Author: Cedric Creusot <cedric.creusot@gmail.com>
 *
 * Created on September 18, 2011, 4:53 PM
 */

#include <iostream>
#include <string>
#include <vector>
#include "Gain.hpp"
#include "Node.hpp"
#include "Data.hpp"

ANode * ID3(Data *);

Attributes select(float outlook, float temperature, float humidity, float wind)
{
    std::cout << "outlook : " << outlook;
    std::cout << " temperature : " << temperature;
    std::cout << " humidity : " << humidity;
    std::cout << " wind : " << wind << std::endl;
    if (outlook <= 0.f && temperature <= 0.f && humidity <= 0.f && wind <= 0.f)
        return PLAYBALL;
    if (outlook > temperature && outlook > humidity && outlook > wind)
        return OUTLOOK;
    if (temperature > outlook && temperature > humidity && temperature > wind)
        return TEMPERATURE;
    if (humidity > temperature && humidity > outlook && humidity > wind)
        return HUMIDITY;
    if (wind > temperature && wind > humidity && outlook < wind)
        return WIND;
}

ANode * outlookNode(Data & data) {
    std::cout << "Outlook Node" << std::endl;
    Node<Outlook> * tmpnode = new Node<Outlook > ();
    tmpnode->AddChild(Sunny, ID3(data.clean<OUTLOOK, Outlook, Sunny>()));
    tmpnode->AddChild(Rain, ID3(data.clean<OUTLOOK, Outlook, Rain>()));
    tmpnode->AddChild(Overcast, ID3(data.clean<OUTLOOK, Outlook, Overcast>()));
    return (tmpnode);
}

ANode * temperatureNode(Data & data) {
    std::cout << "Temperature Node" << std::endl;
    Node<Temperature> * tmpnode = new Node<Temperature > ();
    tmpnode->AddChild(Hot, ID3(data.clean<TEMPERATURE, Temperature, Hot>()));
    tmpnode->AddChild(Mild, ID3(data.clean<TEMPERATURE, Temperature, Mild>()));
    tmpnode->AddChild(Cool, ID3(data.clean<TEMPERATURE, Temperature, Cool>()));
    return (tmpnode);
}

ANode * humidityNode(Data & data) {
    std::cout << "Humidity Node" << std::endl;
    Node<Humidity> * tmpnode = new Node<Humidity > ();
    tmpnode->AddChild(High, ID3(data.clean<HUMIDITY, Humidity, High>()));
    tmpnode->AddChild(Normal, ID3(data.clean<HUMIDITY, Humidity, Normal>()));
    return (tmpnode);
}

ANode * windNode(Data & data) {
    std::cout << "Wind Node" << std::endl;
    Node<Wind> * tmpnode = new Node<Wind > ();
    tmpnode->AddChild(Strong, ID3(data.clean<WIND, Wind, Strong>()));
    tmpnode->AddChild(Weak, ID3(data.clean<WIND, Wind, Weak>()));
    return (tmpnode);
}

ANode * playballNode(Data & data) {
    std::cout << "Play Node" << std::endl;
    Node<PlayBall> * tmpnode = new Node<PlayBall > ();
    if (data.checkOnlyPlayball(Yes))
        tmpnode->AddChild(Yes, NULL);
    else
        tmpnode->AddChild(No, NULL);
    return (tmpnode);
}

typedef ANode * (*func_t)(Data &);

ANode * ID3(Data * data)
{
    static std::map<Attributes, func_t> functions;
    functions[OUTLOOK] = &outlookNode;
    functions[TEMPERATURE] = &temperatureNode;
    functions[HUMIDITY] = &humidityNode;
    functions[WIND] = &windNode;
    functions[PLAYBALL] = &playballNode;
    if ((*data).empty())
        return (NULL);
    float outlook = ((*data)._outlook.empty()) ? (-1) : Gain<Outlook, PlayBall>((*data)._outlook, (*data)._playball).result();
    float temperature = ((*data)._temperature.empty()) ? (-1) : Gain<Temperature, PlayBall>((*data)._temperature, (*data)._playball).result();
    float humidity = ((*data)._humidity.empty()) ? (-1) : Gain<Humidity, PlayBall>((*data)._humidity, (*data)._playball).result();
    float wind = ((*data)._wind.empty()) ? (-1) : Gain<Wind, PlayBall>((*data)._wind, (*data)._playball).result();
    
    ANode * node = NULL;
    Attributes i = select(outlook, temperature, humidity, wind);
    node = functions[i](*data);
    delete data;
    return node;
}

Node<PlayBall> * result(ANode * tree, Outlook out, Temperature temp, Humidity hum, Wind wind)
{
    Node<Outlook> * o = dynamic_cast<Node<Outlook> *>(tree);
    Node<Temperature> *  t = dynamic_cast<Node<Temperature> *>(tree);
    Node<Humidity> * h = dynamic_cast<Node<Humidity> *>(tree);
    Node<Wind> * w = dynamic_cast<Node<Wind> *>(tree);
    
    if (o != NULL)
        return result(o->getChilds()[out], out, temp, hum, wind);
    else if (t != NULL)
        return result(t->getChilds()[temp], out, temp, hum, wind);
    else if (h != NULL)
        return result(h->getChilds()[hum], out, temp, hum, wind);
    else if (w != NULL)
        return result(w->getChilds()[wind], out, temp, hum, wind);
    else
    {
        return dynamic_cast<Node<PlayBall> *>(tree);
    }
}

std::string const & exemple(ANode * tree, Outlook out, Temperature temp, Humidity hum, Wind wind)
{
    static Data dat;
    static std::string yes = "Yes";
    static std::string no = "No";
    static std::string res = no;

    Node<PlayBall> * play = result(tree, out, temp, hum, wind);
    if (play->getChilds().find(Yes) != play->getChilds().end())
        res = yes;
    else
        res = no;
    return res;
}

void freeTree(ANode * tree)
{
    Node<Outlook> * o = dynamic_cast<Node<Outlook> *>(tree);
    Node<Temperature> *  t = dynamic_cast<Node<Temperature> *>(tree);
    Node<Humidity> * h = dynamic_cast<Node<Humidity> *>(tree);
    Node<Wind> * w = dynamic_cast<Node<Wind> *>(tree);
    
    if (o != NULL)
    {
        std::map<Outlook, ANode *>::iterator it = o->getChilds().begin();
        for (; it != o->getChilds().end(); ++it)
            freeTree(it->second);
    }
    else if (t != NULL)
    {
        std::map<Temperature, ANode *>::iterator it = t->getChilds().begin();
        for (; it != t->getChilds().end(); ++it)
            freeTree(it->second);
    }
    else if (h != NULL)
    {
        std::map<Humidity, ANode *>::iterator it = h->getChilds().begin();
        for (; it != h->getChilds().end(); ++it)
            freeTree(it->second);
    }
    else if (w != NULL)
    {
        std::map<Wind, ANode *>::iterator it = w->getChilds().begin();
        for (; it != w->getChilds().end(); ++it)
            freeTree(it->second);
    }
    delete tree;
}

/*
 * 
 */
int main(int argc, char** argv) {
    Data * dat = new Data();
    
    ANode * root = ID3(dat);
    std::cout << "Should I go play ? " << exemple(root, Rain, Cool, High, Strong ) << std::endl;
    freeTree(root);
    return 0;
}

