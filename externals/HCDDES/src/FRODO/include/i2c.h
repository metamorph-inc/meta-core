#ifndef __FRODO_I2C_H__
#define __FRODO_I2C_H__


#ifdef __cplusplus
extern "C" {
#endif


// I2C Interface Functions
bool I2C_Initialize( void* error_handler );
bool I2C_Shutdown( void );
I2C_State I2C_State( void );
bool I2C_Recover( I2C_Channel channel, I2C_State state );
I2C_token I2C_Expect( I2C_Channel channel, unsigned int msgID, unsigned int sizeBytes, float timeLowerBound, float timeUpperBound ); 
bool I2C_Send( I2C_Token token, void* data );
bool I2C_Receive( IC2_Token token, void* buffer );


#ifdef __cplusplus
}
#endif


#endif // __FRODO_I2C_H__

