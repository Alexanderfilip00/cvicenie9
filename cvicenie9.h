#pragma once


#include <QtWidgets>
#include "ui_cvicenie9.h"
#include <iostream> 
#include <iomanip>
#include <QString>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

class Customer {
private:
	QString name, surname;
	float budget;
	int* cart;
public:
	Customer();
	Customer(QString I_name, QString I_surname, float I_budget, int n);
	void SetCart(int id, int n) { cart[id] = n; };
	void SubBudget(float n) { budget -= n; };
	void SetBudget(float n) { budget = n; };
	void SetName(QString n) { name = n; };
	void SetSurname(QString s) { surname = s; };
	void SetupCart(int n);
	QString GetName() { return name; };
	QString GetSurname() { return surname; };
	float Budget() { return budget; };
	int Cart(int I_id) { return cart[I_id]; };
};

class Product {
private:
	int id;
	QString name;
	QString producer;
	int stock;
	int price;
public:
	Product();
	Product(int I_id, QString I_name, QString I_producer, int I_stock, float I_price);
	void Set(int I_id, QString I_name, QString I_producer, int I_stock, float I_price);
	void SetId(int n) { id = n; };
	void SetStock(int n) { stock = n; };

	int Id() { return id; };
	QString Name() { return name; };
	QString Producer() { return producer; };
	int Stock() { return stock; };
	int Price() { return price; };
};

class Eshop {
private:
	QVector<Product> List;
public:
	Eshop() {};
	void ResizeList(int n) { List.resize(n); };
	void SetListItem(int I_id, QString I_name, QString I_producer, int I_stock, float I_price) { List[I_id].Set(I_id, I_name, I_producer, I_stock, I_price); };
	void SetListId(int id,int I_id) { List[id].SetId(I_id); };
	void SetListStock(int id, int n) { List[id].SetStock(n); };
	int GetListStock(int id) { return List[id].Stock(); };
	int GetListPrice(int id) { return List[id].Price(); };
	int GetListId(int id) { return List[id].Id(); };
	QString GetListName(int id) { return List[id].Name(); };
	QString GetListProducer(int id) { return List[id].Producer(); };
};


class cvicenie9 : public QMainWindow
{
    Q_OBJECT

public:
    cvicenie9(QWidget *parent = Q_NULLPTR);
	//~cvicenie9() ;

private:
    Ui::cvicenie9Class ui;
	Eshop* MojObchod = nullptr;
	int* Inventar;
	Customer Zakaznik;
	int* vysledky;
	int* konvertor = nullptr;
	int prekonvertuj(int n);
	void UpravKatalog();
private slots:
	void on_actionOpen_triggered();
	void on_Meno_textChanged();
	void on_Priezvisko_textChanged();
	void on_VRozpocet_valueChanged();
	void on_Vstup_clicked();
	void on_Katalog_itemSelectionChanged();
	void on_PridatKos_clicked();
	void on_NakupKusy_valueChanged();
	void on_Vyprazdnit_clicked();
	void on_Produkt_textChanged();
	void on_Vyrobca_textChanged();
	void on_Zaplatit_clicked();
	void on_actionSave_triggered();
};




