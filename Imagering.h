void SaveAsPicture();
void DrawWindo( GWorldPtr off );
Boolean CheckKey (unsigned char *PtrToKeyMap, short theKey);
void MakeMenu();
void DieError( const unsigned char *errString, OSErr err );
void DieError( const unsigned char *errString );
void StartMakingMovie();
void DrawIntoMain(GWorldPtr off);
void CheckMovieError();
voidPtr MovieThread( void );