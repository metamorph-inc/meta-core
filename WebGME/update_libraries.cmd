echo off
echo Updating WebGME libraries: 
date /t 
echo %TIME% 
where npm 
where node 
if %ERRORLEVEL% == 0 (
    echo #### npm install #### 
	npm install 
    if %ERRORLEVEL% == 0 (
        echo #### npm install webgme #### 
        npm install webgme 
        if %ERRORLEVEL% == 0 (
            echo #### npm install webgme-domain-tools #### 
            npm install webgme-domain-tools 
            if %ERRORLEVEL% == 0 (
                echo #### npm install webgme-cyphy #### 
                npm install webgme-cyphy 
                if %ERRORLEVEL% == 0 (
                    echo update_libraries complete!
                ) else (
                    echo ERROR: Failed to update libraries!
                )
            ) else (
                echo ERROR: npm install webgme-domain-tools failed!
                pause
            )           
        ) else (
            echo ERROR: npm install webgme failed!
            pause
        )
    ) else (
        echo ERROR: npm install failed!
        pause
    )
) else (
    echo ERROR: npm and/or node is not in path! Make sure to install node and select the npm option. 
	pause
)