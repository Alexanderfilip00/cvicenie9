#include "cvicenie9.h"

cvicenie9::cvicenie9(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void cvicenie9::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open file", "", tr("Txt file (*.txt);;All files(*.)"));

	if (fileName.isEmpty()) { return; }

	////qDebug() << fileName;
	QFileInfo fi(fileName);
	////qDebug() << fi.completeSuffix();

	if (fi.suffix() == "txt") {
		QFile FileProdukty(fileName);
		if (!FileProdukty.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			////qDebug() << "Chyba suboru s produktami";
			FileProdukty.close();
			return;
		}


		if (MojObchod != nullptr) {
			delete MojObchod;
			////qDebug() << "MojObchod uz bol nacitany.";
		}
		MojObchod = new Eshop;

		int i;
		short int pocet;
		int id, stock;
		float price;
		QString I_name, I_producer;
		bool ok;

		QTextStream in(&FileProdukty);      //temporary premenna na citanie riadkov
		QString riadok = in.readLine();
		pocet = riadok.toInt(&ok);
		if (!ok) {
			//qDebug() << "Chyba konverzie.";
			exit(1);
		}

		MojObchod->ResizeList(pocet + 1);			//....vytvoreny jeden sample produkt, kde si ulozim pocet produktov		
		Zakaznik.SetupCart(pocet+1);
		Inventar = new int[pocet];

		for (i = 1; i <= pocet; i++) {
			riadok = in.readLine();
			id = riadok.toInt(&ok);
			if (!ok) {
				//qDebug() << "Chyba konverzie.";
				exit(1);
			}
			I_name = in.readLine();
			I_producer = in.readLine();
			riadok = in.readLine();
			stock = riadok.toInt(&ok);
			if (!ok) {
				//qDebug() << "Chyba konverzie.";
				exit(1);
			}
			riadok = in.readLine();
			price = riadok.toInt(&ok);
			if (!ok) {
				//qDebug() << "Chyba konverzie.";
				exit(1);
			}
			Inventar[i - 1] = stock;	//potrebujem to pre funkciu na vyprazdnenie kosika
			MojObchod->SetListItem(id, I_name, I_producer, stock, price);
			Zakaznik.SetCart(0, pocet);
	
			ui.Katalog->setRowCount(pocet);
			ui.Katalog->setColumnCount(5);
			ui.Katalog->setSelectionBehavior(QAbstractItemView::SelectRows);

			QTableWidgetItem* TableId = new QTableWidgetItem();
			TableId->setText(QString::number(id));
			QTableWidgetItem* TableName = new QTableWidgetItem();
			TableName->setText(I_name);
			QTableWidgetItem* TableProducer = new QTableWidgetItem();
			TableProducer->setText(I_producer);
			QTableWidgetItem* TableStock = new QTableWidgetItem();
			TableStock->setText(QString::number(stock));
			QTableWidgetItem* TablePrice = new QTableWidgetItem();
			TablePrice->setText(QString::number(price));

			ui.Katalog->setItem(i-1, 0, TableId);
			ui.Katalog->setItem(i-1, 1, TableName);
			ui.Katalog->setItem(i-1, 2, TableProducer);
			ui.Katalog->setItem(i-1, 3, TableStock);
			ui.Katalog->setItem(i-1, 4, TablePrice); 
		}

		FileProdukty.close();
		MojObchod->SetListId(0,pocet);		//pocitadlo mnozstva produktov

	}
}

Product::Product()
{
	id = 0;
	name = "";
	producer = "";
	stock = 0;
	price = 0.0;
}

Product::Product(int I_id, QString I_name, QString I_producer, int I_stock, float I_price)
{
	id = I_id;
	name = I_name;
	producer = I_producer;
	stock = I_stock;
	price = I_price;
}

void Product::Set(int I_id, QString I_name, QString I_producer, int I_stock, float I_price)
{
	id = I_id;
	name = I_name;
	producer = I_producer;
	stock = I_stock;
	price = I_price;
}

Customer::Customer()
{
	name = "";
	surname = "";
	budget = 0;
	cart = new int[1];
	cart[0] = 0;
}

Customer::Customer(QString I_name, QString I_surname, float I_budget, int n)
{
	int i;
	name = I_name;
	surname = I_surname;
	budget = I_budget;
	cart = new int[n + 1];
	for (i = 0; i < n + 1; i++) {
		cart[i] = 0;		//nastavi prazdny kosik
	}
}

void cvicenie9::on_Meno_textChanged()
{
	if (ui.Priezvisko->text()!="" && ui.VRozpocet->value() != 0.0 && ui.Meno->text()!="") {
		ui.Vstup->setEnabled(true);
	}
	else ui.Vstup->setEnabled(false);
}

void cvicenie9::on_Priezvisko_textChanged()
{
	if (ui.Priezvisko->text() != "" && ui.VRozpocet->value() != 0.0 && ui.Meno->text() != "") {
		ui.Vstup->setEnabled(true);
	}
	else ui.Vstup->setEnabled(false);
}

void cvicenie9::on_VRozpocet_valueChanged()
{
	if (ui.Priezvisko->text() != "" && ui.VRozpocet->value() != 0.0 && ui.Meno->text() != "") {
		ui.Vstup->setEnabled(true);
	}
	else ui.Vstup->setEnabled(false);
}

void cvicenie9::on_Vstup_clicked()
{
	if (ui.Katalog->rowCount() == 0) {
		QMessageBox error;
		error.setText("Pred vstupom od obchodu je potrebne nacitat produkty.");
		error.exec();
	}
	else {
		
		Zakaznik.SetName(ui.Meno->text());
		Zakaznik.SetSurname(ui.Priezvisko->text());
		Zakaznik.SetBudget(ui.VRozpocet->value());
		ui.ZRozpocet->setValue(ui.VRozpocet->value());

		ui.Produkt->setEnabled(true);
		ui.Vyrobca->setEnabled(true);
		ui.Katalog->setEnabled(true);
		ui.Kosik->setEnabled(true);
		ui.Meno->setEnabled(false);
		ui.Priezvisko->setEnabled(false);
		ui.Produkt->setEnabled(true);
		ui.Vyrobca->setEnabled(true);
		ui.VRozpocet->setEnabled(false);
		ui.Vstup->setEnabled(false);
		ui.ZRozpocet->setEnabled(true);
		ui.Zaplatit->setEnabled(true);
		ui.Vyprazdnit->setEnabled(true);
		ui.Suma->setEnabled(true);
		ui.PridatKos->setEnabled(true);
		ui.NakupCena->setEnabled(true);
		ui.NakupKusy->setEnabled(true);
	}

	konvertor = new int[Zakaznik.Cart(0) + 1];
	int i;
	for (i = 1; i <= Zakaznik.Cart(0); i++) {
		konvertor[i] = i;
	}
	konvertor[0] = Zakaznik.Cart(0);
}

void cvicenie9::on_Katalog_itemSelectionChanged()
{
	ui.NakupCena->setValue(ui.NakupKusy->value() * MojObchod->GetListPrice(prekonvertuj(ui.Katalog->currentRow() +1)));
}

/*cvicenie9::~cvicenie9()
{
	delete[] Inventar;
	delete[] MojObchod;
	delete[] vysledky;
	delete[] konvertor;
}*/

int cvicenie9::prekonvertuj(int n) {
	//qDebug() << "vstupne ID: " << n;
	//qDebug() << "vystupne ID: " << konvertor[n];
	return konvertor[n];		//prekonvertuje medzi front-end a back-end ID
}

void cvicenie9::on_PridatKos_clicked()
{
	int id = prekonvertuj(ui.Katalog->currentRow()+1);	//temp premenna na prepocitanie ID
	if (ui.NakupKusy->value() == 0) {				//test, ci nie je pozadovanych 0 produktov

	}
	else {
		if (ui.NakupKusy->value() > MojObchod->GetListStock(id)) {			//test, ci je dost tovaru na sklade
			qDebug() << "Nedostatok tovaru na sklade.";

		}

		else if (ui.NakupCena->value() > Zakaznik.Budget()) {			//test, ci ma zakaznik dost penazi
			qDebug() << "Nedostatocny rozpocet";
		}
		else {					//ak vsetko funguje
			int i;
			Zakaznik.SetCart(id, Zakaznik.Cart(id) + ui.NakupKusy->value());		//prida produkty zakaznikovi do kosika
			qDebug() << "ID:" << id;
			MojObchod->SetListStock(id, MojObchod->GetListStock(id) - ui.NakupKusy->value());		//upravi skladove zasoby obchodu
			//setMainTable(id);				//aktualizuje skladove zasoby
			UpravKatalog();
			Zakaznik.SubBudget(ui.NakupCena->value());

			ui.ZRozpocet->setValue(Zakaznik.Budget());
			ui.Suma->setValue(ui.VRozpocet->value() - Zakaznik.Budget());

			int CartLength = 0;
			int pocitadlo = 0;
			for (i = 1; i <= Zakaznik.Cart(0); i++) {
				if(Zakaznik.Cart(i) != 0) CartLength++;
			}

			ui.Kosik->setRowCount(CartLength);
			ui.Kosik->setColumnCount(5);

			ui.Kosik->setSelectionBehavior(QAbstractItemView::SelectRows);
			for (i = 1; i <= Zakaznik.Cart(0); i++) {

				if ( Zakaznik.Cart(i) != 0 ) {
					QTableWidgetItem* TableId = new QTableWidgetItem();
					TableId->setText(QString::number(i));
					QTableWidgetItem* TableName = new QTableWidgetItem();
					TableName->setText(MojObchod->GetListName(i));
					QTableWidgetItem* TableProducer = new QTableWidgetItem();
					TableProducer->setText(MojObchod->GetListProducer(i));
					QTableWidgetItem* TableStock = new QTableWidgetItem();
					TableStock->setText(QString::number(Zakaznik.Cart(i)));
					QTableWidgetItem* TablePrice = new QTableWidgetItem();
					TablePrice->setText(QString::number(MojObchod->GetListPrice(i)));

					ui.Kosik->setItem(pocitadlo, 0, TableId);
					ui.Kosik->setItem(pocitadlo, 1, TableName);
					ui.Kosik->setItem(pocitadlo, 2, TableProducer);
					ui.Kosik->setItem(pocitadlo, 3, TableStock);
					ui.Kosik->setItem(pocitadlo, 4, TablePrice);
					pocitadlo++;
				}
			}
			ui.NakupKusy->setValue(0);
			ui.NakupCena->setValue(0);
		}


	}


	
	
}

void cvicenie9::on_NakupKusy_valueChanged()
{
	ui.NakupCena->setValue(MojObchod->GetListPrice(prekonvertuj(ui.Katalog->currentRow() + 1)) * ui.NakupKusy->value());
}

void cvicenie9::on_Vyprazdnit_clicked()
{
	int i;
	Zakaznik.SetBudget(ui.VRozpocet->value());
	ui.ZRozpocet->setValue(ui.VRozpocet->value());
	ui.Suma->setValue(0);
	ui.Kosik->setRowCount(0);
	for (i = 1; i <= Zakaznik.Cart(0); i++) {	//vynulujem zaznam nakupov
		Zakaznik.SetCart(i, 0);
	}
	

	for (i = 0; i < MojObchod->GetListId(0); i++) {
		MojObchod->SetListStock(i+1, Inventar[i]);
	}
	UpravKatalog();
	ui.Produkt->setText("");
	ui.Produkt->setText("");


}

void cvicenie9::on_Produkt_textChanged()
{
	if (konvertor != nullptr) delete[] konvertor;
	konvertor = new int[Zakaznik.Cart(0) + 1];
	int i, n;

	if (ui.Produkt->text() == "" && ui.Vyrobca->text() == "") {
		qDebug() << "prazdne vyhladavanie produktu";
		for (i = 0; i <= Zakaznik.Cart(0); i++) {
			konvertor[i] = i;
		}
		konvertor[0] = Zakaznik.Cart(0);
	}
	else {
		for (i = 0; i <= Zakaznik.Cart(0); i++) {
			konvertor[i] = 0;			//deklaracia pocitadla spravnych vysledkov
		}

		for (i = 1; i <= Zakaznik.Cart(0); i++) {

			if(MojObchod->GetListName(i).contains(ui.Produkt->text(), Qt::CaseInsensitive) && MojObchod->GetListProducer(i).contains(ui.Vyrobca->text(), Qt::CaseInsensitive)){
				konvertor[0]++;										//pocet spravnych vysledkov sa zvacsi o 1
				konvertor[konvertor[0]] = i;							//na dane miesto sa zapise id najdeneho produktu
			}
		}	
	}
	UpravKatalog();
}

void cvicenie9::on_Vyrobca_textChanged()
{
	if (konvertor != nullptr) delete[] konvertor;
	konvertor = new int[Zakaznik.Cart(0) + 1];
	int i, n;

	if (ui.Vyrobca->text() == "" && ui.Produkt->text() == "") {
		for (i = 0; i <= Zakaznik.Cart(0); i++) {
			konvertor[i] = i;
		}
		konvertor[0] = Zakaznik.Cart(0);
	}
	else {
		for (i = 0; i <= Zakaznik.Cart(0); i++) {
			konvertor[i] = 0;			//deklaracia pocitadla spravnych vysledkov
		}

		for (i = 1; i <= Zakaznik.Cart(0); i++) {

			if (MojObchod->GetListName(i).contains(ui.Produkt->text(), Qt::CaseInsensitive) && MojObchod->GetListProducer(i).contains(ui.Vyrobca->text(), Qt::CaseInsensitive)) {
				konvertor[0]++;										//pocet spravnych vysledkov sa zvacsi o 1
				konvertor[konvertor[0]] = i;							//na dane miesto sa zapise id najdeneho produktu

			}
		}

	}

	UpravKatalog();
}

void cvicenie9::on_Zaplatit_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save file", "", tr("Txt File (*.txt);;All files (*.)"));
	if (fileName.isEmpty()) { return; }

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	out << "meno: " << Zakaznik.GetName() << " \npriezvisko: " << Zakaznik.GetSurname() << " \n\n";
	out << "Nakupene produkty: \nid" << QString::fromStdString(std::string(10, ' ')) << "nazov" << QString::fromStdString(std::string(5, ' ')) << "vyrobca" << QString::fromStdString(std::string(5, ' ')) << "zasoba" << QString::fromStdString(std::string(5, ' ')) << "cena\n";


	for (int i = 1; i <= Zakaznik.Cart(0); i++) 
	{
		i = prekonvertuj(i);
		if (Zakaznik.Cart(i) != 0) {
			out << QString::number(MojObchod->GetListId(i)).rightJustified(2, ' ') << MojObchod->GetListName(i).rightJustified(15, ' ') << MojObchod->GetListProducer(i).rightJustified(12, ' ') << QString::number(Zakaznik.Cart(i)).rightJustified(11, ' ') << QString::number(MojObchod->GetListPrice(i)).rightJustified(9, ' ') << "\n";
		}
	}

	out << "\ncena nakupu: " << QString::number(ui.Suma->value(), 'f', 2) << "\ncena bez DPH: " << QString::number(ui.Suma->value() * 0.8, 'f', 2) << "\n";

	file.close();
	exit(0);
}

void cvicenie9::on_actionSave_triggered()
{
	ui.Zaplatit->click();
}

void cvicenie9::UpravKatalog() {
	int i;
	ui.Katalog->setRowCount(konvertor[0]);
	int pocitadlo = 0;
	for (i = 1; i <= konvertor[0]; i++) {
		if (konvertor[i] != 0) {
			
			////qDebug() << "konvertor[i]:" << konvertor[i];
			QTableWidgetItem* TableId = new QTableWidgetItem();
			TableId->setText(QString::number(pocitadlo+1));
			QTableWidgetItem* TableName = new QTableWidgetItem();
			TableName->setText(MojObchod->GetListName(konvertor[i]));
			QTableWidgetItem* TableProducer = new QTableWidgetItem();
			TableProducer->setText(MojObchod->GetListProducer(konvertor[i]));
			QTableWidgetItem* TableStock = new QTableWidgetItem();
			TableStock->setText(QString::number(MojObchod->GetListStock(konvertor[i])));
			QTableWidgetItem* TablePrice = new QTableWidgetItem();
			TablePrice->setText(QString::number(MojObchod->GetListPrice(konvertor[i])));

			ui.Katalog->setItem(pocitadlo, 0, TableId);
			ui.Katalog->setItem(pocitadlo, 1, TableName);
			ui.Katalog->setItem(pocitadlo, 2, TableProducer);
			ui.Katalog->setItem(pocitadlo, 3, TableStock);
			ui.Katalog->setItem(pocitadlo, 4, TablePrice);
			pocitadlo++;
		}
	}
}

void Customer::SetupCart(int n) {
	cart = new int[n];
	int i;
	for (i = 0; i < n; i++) 
		cart[i] = 0;

}