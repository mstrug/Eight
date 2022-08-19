#ifndef LOGGER_H_
#define LOGGER_H_

#include <e32std.h>
#include <f32file.h> 

//#ifdef _DEBUG

class Logger
	{
public:	
	
	static void LogNum( TPtrC aFile, TInt aNum, TBool aAppend = ETrue )
		{
		RFs fs;
		fs.Connect();
		RFile file;
		TInt err = 0;
		if ( aAppend )
			{
			err = file.Open( fs, aFile, EFileWrite );
			if ( err == KErrNone )
				{
				TInt a = 0;
				file.Seek( ESeekEnd, a );
				}
			}
		if ( !aAppend || err != KErrNone )
			{
			file.Replace( fs, aFile, EFileWrite );
			}
		TBuf8<64> bf;
		bf.Num( aNum );
		bf.Append( _L("\r\n") );
		file.Write( bf );
		file.Close();
		fs.Close();		
		}

	static void LogStrNum( TPtrC aFile, TPtrC aMessage, TInt aNum, TBool aAppend = ETrue )
		{
		RFs fs;
		fs.Connect();
		RFile file;
		TInt err = 0;
		if ( aAppend )
			{
			err = file.Open( fs, aFile, EFileWrite );
			if ( err == KErrNone )
				{
				TInt a = 0;
				file.Seek( ESeekEnd, a );
				}
			}
		if ( !aAppend || err != KErrNone )
			{
			file.Replace( fs, aFile, EFileWrite );
			}
		RBuf8 bf;
		bf.CreateL( aMessage.Length() + 20 );
		bf.Append( aMessage );
		bf.AppendNum( aNum );
		bf.Append( _L("\r\n") );
		file.Write( bf );
		bf.Close();
		file.Close();
		fs.Close();		
		}
	
	static void LogStr( TPtrC aFile, TPtrC aMessage, TBool aAppend = ETrue )
		{
		RFs fs;
		fs.Connect();
		RFile file;
		TInt err = 0;
		if ( aAppend )
			{
			err = file.Open( fs, aFile, EFileWrite );
			if ( err == KErrNone )
				{
				TInt a = 0;
				file.Seek( ESeekEnd, a );
				}
			}
		if ( !aAppend || err != KErrNone )
			{
			file.Replace( fs, aFile, EFileWrite );
			}
        RBuf8 bf;
        bf.CreateL( aMessage.Length() + 20 );
		bf.Append( aMessage );
		bf.Append( _L("\r\n") );
		file.Write( bf );
		bf.Close();
		file.Close();
		fs.Close();		
		}

	static void LogStr( TPtrC aFile, TPtrC8 aMessage, TBool aAppend = ETrue )
		{
		RFs fs;
		fs.Connect();
		RFile file;
		TInt err = 0;
		if ( aAppend )
			{
			err = file.Open( fs, aFile, EFileWrite );
			if ( err == KErrNone )
				{
				TInt a = 0;
				file.Seek( ESeekEnd, a );
				}
			}
		if ( !aAppend || err != KErrNone )
			{
			file.Replace( fs, aFile, EFileWrite );
			}
		file.Write( aMessage );
		file.Close();
		fs.Close();		
		}
	
	struct SElement1
		{
        TTimeIntervalMicroSeconds iInterval;
        TUint32 iTick;
		TInt iVal1;
		};
	
	static void LogIntervalArray( TPtrC aFile, RArray< SElement1 >& aArray )
		{
		RFs fs;
		fs.Connect();
		RFile file;
		file.Replace( fs, aFile, EFileWrite );
		TBuf8<32> bf;
		for ( TInt i = 0; i < aArray.Count(); i++ )
			{
			bf.Zero();
            bf.AppendNum( aArray[ i ].iTick );
//            bf.AppendNum( aArray[ i ].iInterval.Int64() );
			bf.Append( _L8("\t") );
			bf.AppendNum( aArray[ i ].iVal1 );
			bf.Append( _L8("\r\n") );
			file.Write( bf );
			}
		file.Close();
		fs.Close();		
		}
	};

#ifdef _LOGGER_STRUCT_

RArray< TLoggerStruct > LoggerStructArray( 1024 * 10 );

class TLoggerStruct
    {
public:
    TBuf<16> iMessage;
    TUint32 iTick;
    TLoggerStruct( const TDesC& aMessage )
        {
        ASSERT( aMessage.Length <= 16 );
        iMessage.Copy( aMessage );
        iTick = User::NTickCount();
        }
    };

void SaveLoggerStructArray( TPtrC aFile )
    {
    RFs fs;
    fs.Connect();
    RFile file;
    TInt err = 0;
    file.Replace( fs, aFile, EFileWrite );

    for ( TInt i = 0; i < LoggerStructArray.Count(); i++ )
        {
        TBuf8<64> bf;
        bf.Copy( LoggerStructArray[ i ].iMessage );
        bf.Append( _L("\t\t") );
        bf.AppendNum( LoggerStructArray[ i ].iTick );
        bf.Append( _L("\r\n") );
        file.Write( bf );
        }
    
    file.Close();
    fs.Close();     
    }

#endif

//#endif

#endif /*LOGGER_H_*/
