#if !defined(__BORDAO_H)
#define __BORDAO_H

#include "autobase.h"
#include <dbdaoint.h>
#undef EOF

#define RProp(name, type) \
	type Get##name() const; \
	type __declspec(property(get=Get##name)) name
							//Read only non-indexed property

#define WProp(name, type) \
	void Set##name(type); \
	type __declspec(property(put=Set##name)) name
							//Write only non-indexed property

#define RWProp(name, type) \
	type Get##name() const; \
	void Set##name(type); \
	type __declspec(property(get=Get##name, put=Set##name)) name 
							//Read-write non-indexed property

#define IRProp(name, type, indtype) \
	type Get##name(indtype) const; \
	type __declspec(property(get=Get##name)) name[] 
							//Read only indexed property

#define IWProp(name, type, indtype) \
	void Set##name(type, indtype); \
	type __declspec(property(get=Set##name)) name[] 
							//Write only indexed property

#define IRWProp(name, type, indtype) \
	type Get##name(indtype) const; \
	void Set##name(type, indtype); \
	type __declspec(property(get=Get##name, put=Set##name)) name[] 
							//Read-write indexed property

class Recordset;
class Property;

class Field : public DaoBase {	//Proxy class for DAOField
public:
	Field(DAOField* = 0);
	Field(Field&);
	Field(Recordset&, Variant);
	~Field() {};
	Field& operator=(Field const &);
	void AppendChunk(Variant Val);
	Variant GetChunk(long Offset, long Bytes);
	Property CreateProperty(LPCTSTR Name, Variant Type, Variant Value, Variant DDL);
	RProp(CollatingOrder, long);
	RWProp(Name, CString);
	RWProp(Value, Variant);
	RProp(Type, short);
	RProp(Size, long);
	RProp(TableName, CString);
	RProp(Attributes, long);
	RProp(SourceField, CString);
	RProp(SourceTable, CString);
	RWProp(OrdinalPosition, short);
	RWProp(ValidationText, CString);
	RWProp(ValidateOnSet, BOOL);
	RWProp(ValidationRule, CString);
	RWProp(DefaultValue, Variant);
	RWProp(Required, BOOL);
	RWProp(AllowZeroLength, BOOL);
	RProp(DataUpdatable, BOOL);
	RWProp(ForeignName, CString);
	RProp(CollectionIndex, short);
	RProp(OriginalValue, Variant);
	RProp(VisibleValue, Variant);
	RProp(FieldSize, long);
};

class Database;
class QueryDef;
class TableDef;
class Connection;

class Recordset : public DaoBase {		//Proxy class for DAORecordset
public:
	static Database* pdbase;
	Recordset(DAORecordset* = 0);
	Recordset(Recordset&);
	Recordset(LPCSTR);
	Recordset(CString const &);
	~Recordset() {};
	Recordset& operator=(Recordset const &);
	CString GetName();
	void AddNew();
	void Edit();
	void Delete();
	void Update(short = dbUpdateRegular, VARIANT_BOOL = FALSE);
	void CancelUpdate(short = dbUpdateRegular);
	void Close();
	Recordset Open(LONG = -1, LONG = -1);
	void Open(LPCSTR);
	void MoveFirst();
	void MoveNext();
	void MoveLast();
	void MovePrevious();
	void Requery(Variant NewQueryDef);
	Recordset ListFields();
	void FindFirst(LPCTSTR Criteria);
	void FindLast(LPCTSTR Criteria);
	void FindNext(LPCTSTR Criteria);
	void FindPrevious(LPCTSTR Criteria);
	void Seek(LPCTSTR Comparison, Variant Key1, Variant Key2, Variant Key3,
   		Variant Key4, Variant Key5, Variant Key6, Variant Key7, Variant Key8,
		Variant Key9, Variant Key10, Variant Key11, Variant Key12, Variant Key13);
	Recordset Clone();
	void Move(long Rows, Variant StartBookmark = -1);
	void FillCache(long Rows, Variant StartBookmark);
	Recordset CreateDynaset(LPCTSTR Name, LONG Options, Variant Inconsistent);
	Recordset CreateSnapshot(LPCTSTR Source, LONG Options);
	QueryDef CopyQueryDef();
	Recordset ListIndexes();
	Variant GetRows(long NumRows);
	Variant* GetRow(Variant&);
	void Cancel();
	BOOL NextRecordset();
	RWProp(Filter, CString);
	RWProp(Sort, CString);
	RProp(EOF, BOOL);
	RProp(BOF, BOOL);
	RProp(FieldCount, short);
	RProp(Type, short);
	RProp(RecordCount, long);
	RWProp(Bookmark, SAFEARRAY);
	RProp(Bookmarkable, BOOL);
	RProp(DateCreated, DATE);
	RWProp(Index, CString);
	RProp(LastModified, SAFEARRAY);
	RProp(LastUpdated, DATE);
	RWProp(LockEdits, BOOL);
	RProp(NoMatch, BOOL);
	RProp(Transactions, BOOL);
	RProp(Updatable, BOOL);
	RProp(Restartable, BOOL);
	RProp(ValidationText, CString);
	RProp(ValidationRule, CString);
	RWProp(CacheStart, SAFEARRAY);
	RWProp(CacheSize, long);
	RWProp(PercentPosition, float);
	RWProp(AbsolutePosition, long);
	RProp(EditMode, short);
	RProp(ODBCFetchCount, long);
	RProp(ODBCFetchDelay, long);
	RProp(Parent, Database);
	RProp(hStmt, long);
	RProp(StillExecuting, BOOL);
	RWProp(BatchSize, long);
	RProp(BatchCollisionCount, long);
	RProp(BatchCollisions, Variant);
	RWProp(Connections, Connection);
	RProp(RecordStatus, short);
	RWProp(UpdateOptions, long);
	IRWProp(FieldValue, Variant, Variant);
	IRProp(FieldName, CString, int);
	IRProp(FieldType, short, Variant);
	IRProp(FieldSize, long, Variant);
	IRProp(FieldFieldSize, long, Variant);
	IRProp(FieldTableName, CString, Variant);
	IRProp(FieldSource, CString, Variant);
	IRProp(Fields, Field, Variant);
	IRWProp(Collect, Variant, Variant);
};

class DBEngine;
class Relation;

class Database : public DaoBase {		//Proxy class for DAODatabase
public:
	static DBEngine* pdbeng;
	Database(DAODatabase* = 0);
	Database(Database&);
	Database(LPCSTR, LONG = 0, BOOL = FALSE, LPCSTR = NULL);
	~Database() {};
	void SetDatabase();
	Recordset OpenRecordset(LPCTSTR, LONG = -1, LONG = -1, LONG = -1);
	QueryDef CreateQueryDef(LPCTSTR, LPCTSTR);
	TableDef CreateTableDef(LPCTSTR, LONG = 0, LPCTSTR = 0, LPCTSTR = 0);
	void TableDefAppend(TableDef &);
	void TableDefDelete(LPCTSTR Name);
	void Close();
	void Compact(LPCSTR);
	void Execute(LPCTSTR Query, LONG Options);
	Property CreateProperty(LPCTSTR Name, Variant Type, Variant Value);
	Relation CreateRelation(LPCTSTR Name, LPCSTR Table, LPCSTR ForeignTable, LONG Attributes);
	void BeginTrans();
	void CommitTrans(long);
	void Rollback();
	Recordset CreateDynaset(LPCTSTR Name, LONG Options, Variant Inconsistent);
	Recordset CreateSnapshot(LPCTSTR Source, LONG Options);
	void DeleteQueryDef(LPCTSTR Name);
	long ExecuteSQL(LPCTSTR SQL);
	Recordset ListFields(LPCTSTR Name);
	Recordset ListTables();
	QueryDef OpenQueryDef(LPCTSTR Name);
	Recordset OpenTable(LPCTSTR Name, LONG Options);
	void Sinchronize(LPCTSTR DbPathName, LONG ExchangeType);
	void MakeReplica(LPCTSTR PathName, LPCTSTR Description, LONG Options);
	void NewPassword(LPCTSTR Old, LPCTSTR New);
	void PopulatePartial(LPCTSTR DbPathName);
	Database& operator=(Database const &);
	CString GetName();
	RProp(CollatingOrder, long);
	RWProp(Connect, CString);
	RWProp(QueryTimeout, short);
	RProp(Transactions, BOOL);
	RProp(Updatable, BOOL);
	RProp(Version, CString);
	RProp(RecordsAffected, long);
	RProp(ReplicaID, CString);
	RWProp(DesignMasterID, CString);
	RProp(Connections, Connection);
};

class User;
class Group;

class Workspace : public DaoBase {		//Proxy class for DAOWorkspace
public:
	Workspace(DAOWorkspace* = 0);
	Workspace(Workspace&);
	~Workspace() {};
	Workspace& operator=(Workspace const &);
	Database OpenDatabase(LPCTSTR, LONG = 0, BOOL = FALSE, LPCSTR = NULL);
	void BeginTrans();
	void CommitTrans(long = 0);
	void Rollback();
	void Close();
	Database CreateDatabase(LPCTSTR Name, LPCTSTR Connect, LONG Option);
	User CreateUser(LPCTSTR Name, LPCTSTR PID, LPCTSTR Password);
	Group CreateGroup(LPCTSTR Name, LPCTSTR PID);
	Connection OpenConnection(LPCTSTR Name, LONG Options, BOOL ReadOnly, LPCSTR Connect);
	CString GetName();
	RWProp(UserName, CString);
	WProp(Password, CString);
	RWProp(IsolateODBCTrans, short);
	RWProp(LoginTimeout, long);
	RWProp(DefaultCursorDriver, long);
	RProp(hEnv, long);
	RProp(Type, long);
};

class DBEngine : public DaoBase {		//Proxy class for DAODBEngine
public:
	DBEngine();
	~DBEngine();
	Database OpenDatabase(LPCTSTR, LONG = 0, BOOL = FALSE, LPCSTR = NULL);
	Workspace CreateWorkspace(LPCTSTR Name, LPCSTR UserName, LPCSTR Password,
   	WorkspaceTypeEnum UseType);
	void Idle(IdleEnum);
	void CompactDatabase(LPCTSTR SrcName, LPCTSTR DstName, LPCTSTR DstLocale, LONG Options, LPCTSTR SrcLocale);
	void RepairDatabase(LPCTSTR name);
	void RegisterDatabase(LPCTSTR dsn, LPCTSTR driver, BOOL silent, LPCTSTR Attributes);
	Database CreateDatabase(LPCTSTR Name, LPCTSTR Locale, LONG Option = -1);
	void FreeLocks();
	void BeginTrans();
	void CommitTrans(long);
	void Rollback();
	void SetDefaultWorkspace(LPCTSTR Name, LPCTSTR Password);
	void SetDataAccessOption(short Option, Variant Value);
	long ISAMStats(long StatNum, Variant Reset);
	Connection OpenConnection(LPCTSTR Name, LONG Options, BOOL ReadOnly, LPCSTR Connect);
	void SetOption(long Option, Variant Value);
	RProp(Version, CString);
	RWProp(IniPath, CString);
	WProp(DefaultUser, CString);
	WProp(DefaultPassword, CString);
	RWProp(LoginTimeout, short);
	RWProp(SystemDB, CString);
	RWProp(DefaultType, long);
	IRProp(Workspaces, Workspace, Variant);
};

class Parameter : public DaoBase {		//Proxy class for DAOParameter
public:
	Parameter(DAOParameter* = 0);
	Parameter(Parameter&);
	Parameter(QueryDef&, Variant);
	~Parameter() {};
	Parameter& operator=(Parameter const &);
	Parameter& operator=(Variant const &);
	CString GetName();
	RWProp(Value, Variant);
	RWProp(Type, short);
	RWProp(Direction, short);
};

class QueryDef : public DaoBase {		//Proxy class for DAOQueryDef
public:
	QueryDef(DAOQueryDef* = 0);
	QueryDef(QueryDef&);
	QueryDef(Database*, LPCSTR);
	QueryDef(LPCSTR);
	QueryDef(CString const &);
	QueryDef(Database*, CString const &);
	~QueryDef() {};
	QueryDef& QueryDef::operator=(QueryDef const &);
	Recordset OpenRecordset(LONG = -1, LONG = -1, LONG = -1);
	void Execute(LONG = dbFailOnError);
	void Close();
	QueryDef _Copy();
	short Compare(QueryDef&);
	Recordset CreateDynaset(LPCTSTR Name, LONG Options, Variant Inconsistent);
	Recordset CreateSnapshot(LPCTSTR Source, LONG Options);
	Recordset ListParameters();
	Property CreateProperty(LPCTSTR Name, Variant Type, Variant Value);
	void Cancel();
	RProp(FieldCount, short);
	RProp(DateCreated, DATE);
	RProp(LastUpdated, DATE);
	RWProp(Name, CString);
	RWProp(ODBCTimeout, short);
	RProp(Type, short);
	RWProp(SQL, CString);
	RProp(Updatable, BOOL);
	RWProp(Connect, CString);
	RWProp(ReturnsRecords, BOOL);
	RProp(RecordsAffected, long);
	RProp(hStmt, long);
	RWProp(MaxRecords, long);
	RProp(StillExecuting, BOOL);
	RWProp(CacheSize, long);
	RWProp(Prepare, Variant);
	IRProp(Parameters, Parameter, Variant);
};

class Index;

class TableDef : public DaoBase {		//Proxy class for DAOTableDef
public:
	TableDef(DAOTableDef* = 0);
	TableDef(TableDef&);
	~TableDef() {};
	TableDef& TableDef::operator=(TableDef const &);
	Recordset OpenRecordset(LONG = -1, LONG = -1);
	Field CreateField(LPCTSTR, short, LONG = -1);
	void FieldAppend(Field);
	RWProp(Attributes, long);
	RWProp(Connect, CString);
	RProp(DateCreated, DATE);
	RProp(LastUpdated, DATE);
	RWProp(Name, CString);
	RWProp(SourceTableName, CString);
	RProp(Updatable, BOOL);
	RWProp(ValidationText, CString);
	RWProp(ValidationRule, CString);
	RProp(RecordCount, long);
	void RefreshLink();
	Index CreateIndex(LPCTSTR Name);
	Property CreateProperty(LPCTSTR Name, Variant Type, Variant Value);
	RProp(ConflictTable, CString);
	RWProp(ReplicaFilter, Variant);
};

Variant getdate();		//Helper function for retrieving current date

CString shortdate(Variant);		//Date without time

Variant GetField(CString select, CString from, CString where, Variant value);
					//Retrieves field value from table
#endif
