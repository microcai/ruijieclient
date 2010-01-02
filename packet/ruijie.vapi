[CCode(cheader_filename = "supplicant.h")]
namespace ruijie{
	public int start_auth(
			[CCode(array_length = false, array_nullterminated = false)]
			char[]  name,
			[CCode(array_length = false, array_nullterminated = false)]
			char[] passwd,
			[CCode(array_length = false, array_nullterminated = false)]
			char[] nic_name,
			//
			int authmode
			);
	public int stop_auth();
	public int get_server_msg( 
			[CCode(array_length = false, array_nullterminated = false)]
			char * raw_encode_message_out,
			int  length
			);
	public int echo();
}
