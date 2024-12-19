#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    tv = new QTableView;

    dataBase = new QSqlDatabase();

    simpleQuery = new QSqlQuery();

    queryModel = new QSqlQueryModel;
    queryModel->setHeaderData(0, Qt::Horizontal, QObject::tr("Название"));
    queryModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Описание"));
}

DataBase::~DataBase()
{
    delete dataBase;
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

    if(request == requestComedy){
        queryModel->setHeaderData(0,Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1,Qt::Horizontal, tr("Описание фильма"));

        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Comedy'", *dataBase);
    }
    else if(request == requestHorrors){
        queryModel->setHeaderData(0,Qt::Horizontal, tr("Название фильма"));
        queryModel->setHeaderData(1,Qt::Horizontal, tr("Описание фильма"));

        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Horror'", *dataBase);
    }
    emit sig_SendDataFromDB(queryModel, request);


}

/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
