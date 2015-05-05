FOR /F "tokens=* delims= " %%g IN ('dir /s /b instrumented') DO (
	FOR /F "delims= " %%a IN ('dir /s /b %%g\*.dll') DO (
		FOR /F "delims= " %%b IN ('dir /s /b %%~nxa') DO (
			xcopy /Y %%a %%b
		)
	)
)