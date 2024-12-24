#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    tv = new QTableView;

    dataBase = new QSqlDatabase();

    simpleQuery = new QSqlQuery();

    queryModel = new QSqlQueryModel;
    tableModel = new QSqlTableModel(this, *dataBase);

}
DataBase::~DataBase()
{
    delete dataBase;
    delete simpleQuery;
    delete queryModel;
    delete tableModel;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{

    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
    *simpleQuery = QSqlQuery(*dataBase);

}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());


    ///Тут должен быть код ДЗ


    bool status;
    status = dataBase->open( );
    emit sig_SendStatusConnection(status);

}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();

}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(int request)
{
    if(request + 1 == requestAllFilms){
        tableModel->setTable("film");
        tableModel->select();
        emit sig_SendDataFromDB(tableModel, request);
    }
    else if(request + 1 == requestComedy){
        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Comedy'", *dataBase);
        emit sig_SendDataFromDB(queryModel, request);
    }
    else if(request + 1 == requestHorrors){
        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Horror'", *dataBase);
        emit sig_SendDataFromDB(queryModel, request);
    }

}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
