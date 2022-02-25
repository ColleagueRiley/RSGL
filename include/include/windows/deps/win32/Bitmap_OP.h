class Bitmap_Operations
{
public:
	Bitmap_Operations(){}
	
	~Bitmap_Operations(){}
	//initializing of buffers, must be done before all other operations
	// num is the number of buffers that will be used
    void Initialize_Buffers(HWND handle,int num);
	//prepares buffer[which] (1st index is 0) for following operations 
	void Create_Buffer( int which);
	//frees the resources of buffer[which] (1st index = 0)
	void Free_Buffer(int which);
	//frees all resources that have been allocated 
    void Free_Buffers(); 
	//Copies an area from a buffer (=rectangle coordinates->arguments 3-6)
	// to a buffer (coordinates ->arguments 7,8) 
	void Copy_Area_from_to(int frombuf, int tobuf,int top_from_x, int top_from_y,
		int width, int height, int top_to_x, int top_to_y);
	//Similar to Copy_Area_from_to,Copies an area from a buffer to the screen 
	void Copy_Area_to_Screen(int frombuf,int top_from_x, int top_from_y,int width, int height
		,int top_to_x, int top_to_y); 
	// Copies whole content of buffer[which] to screen
	void Copy_to_Screen(int which);
    // returns number of buffers
    int Num_of_Buffers(); 
	// Copies an area defined by rect_from (RECT structure) and an handle (h_from)
	// to the buffer[which]
    void Copy_Surface_to_Buffer(HWND h_from,RECT rect_from,int which);
	HDC & Get_HDC();
	HDC & Get_DC_Buffer(int which);
	 
protected:
	HDC my_DC;
	HDC *my_DC_Buffer;
	HWND my_handle;
	RECT *dc_rect;
	HBITMAP  *hbm_Buffer;
	HBITMAP *hbm_oldBuffer;
    int num_buf;
};