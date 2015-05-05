
#include <vector>

class PathUtil {
	std::vector<CString> 				m_vecPaths;
	bool								m_bArePathsValid;

public:

	PathUtil() : m_bArePathsValid( false ) {}

	std::vector<CString> getPaths() const
	{
		return m_vecPaths;
	}

	bool arePathsValid() const
	{
		return m_bArePathsValid;
	}

	bool loadPaths(IMgaProject* pProject, bool bRefresh)
	{
		CString m_strParadigmPath;
		CString	m_strProjectPath;

		if ( bRefresh ) {
			m_vecPaths.clear();
			m_bArePathsValid = false;

			if ( pProject ) {
				long lStatus;
				COMTHROW( pProject->get_ProjectStatus( &lStatus ) );
				if ( (lStatus & 0x01L) != 0 ) {
					CComBSTR bstrParadigm;
					COMTHROW( pProject->get_ParadigmConnStr( &bstrParadigm ) );
					m_strParadigmPath = CString( bstrParadigm );
					if ( m_strParadigmPath.Find( _T("MGA=") ) == 0 ) {
						int iPos = m_strParadigmPath.ReverseFind( _T('\\') );
						if( iPos >= 4 ) {
							m_strParadigmPath = m_strParadigmPath.Mid( 4, iPos - 4 );
							if( m_strParadigmPath.IsEmpty() )
								m_strParadigmPath = '\\';
						}
					}

					CComBSTR bstrProject;
					COMTHROW( pProject->get_ProjectConnStr( &bstrProject ) );
					m_strProjectPath = CString( bstrProject );
					if ( m_strProjectPath.Find( _T("MGA=") ) == 0 ) {
						int iPos = m_strProjectPath.ReverseFind( _T('\\') );
						if( iPos >= 4 ) {
							m_strProjectPath = m_strProjectPath.Mid( 4, iPos - 4 );
							if( m_strProjectPath.IsEmpty() )
								m_strProjectPath = '\\';
						}
					}
				}
			}
		}

		if ( ! m_bArePathsValid ) {

			CString strPath;
			try {
				CComPtr<IMgaRegistrar> spRegistrar;
				COMTHROW( spRegistrar.CoCreateInstance( OLESTR( "Mga.MgaRegistrar" ) ) );
				CComBSTR bstrPath;
				COMTHROW( spRegistrar->get_IconPath( REGACCESS_BOTH, &bstrPath ) );

				strPath = bstrPath;
			}
			catch ( hresult_exception & ) {
			}

			strPath.Replace( _T("$PARADIGMDIR"), m_strParadigmPath );
			strPath.Replace( _T("$PROJECTDIR"), m_strProjectPath );

			while( ! strPath.IsEmpty() ) {
				int iPos = strPath.Find( ';' );
				if( iPos == 0) // zolmol: if accidentaly there are two semicolons, or the path starts with a semicolon
				{
					strPath = strPath.Right( strPath.GetLength() - 1 );
					continue;
				}
				CString strDir;
				if ( iPos != -1 ) {
					strDir = strPath.Left( iPos );
					strPath = strPath.Right( strPath.GetLength() - iPos - 1 );
				}
				else {
					strDir = strPath;
					strPath.Empty();
				}
				strDir.Replace( '/', '\\' );
				if ( strDir.GetAt( strDir.GetLength() - 1 ) != '\\' )
					strDir += _T("\\");
				m_vecPaths.push_back( strDir );
			}
			m_vecPaths.push_back( _T(".\\") );

			m_bArePathsValid = true;
		}

		return m_bArePathsValid;
	}
};
