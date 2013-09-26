#define size_x 600
Texture2D input;
RWTexture2D<float4> output;

static const float gaussfilter[7] = { 0.030078323, 0.104983664, 0.222250419, 0.285375187, 0.222250419, 0.104983664, 0.030078323 };

groupshared float4 horizontalpoints[size_x];

[numthreads(size_x, 1, 1)]
void main( uint3 dispatchThreadID : SV_DispatchThreadID )
{
	//load the current data from input texture
	float4 data = input.Load(dispatchThreadID);

	//Store the data into the GSM for the current thread
	horizontalpoints[dispatchThreadID.x] = data;

	//Syncgronize all threads
	GroupMemoryBarrierWithGroupSync();

	//offset the texture lacation to the first sample location
	int3 textureLocation = dispatchThreadID - int3(3, 0, 0);

	float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	for (int x = 0; x < 7; x++)
		color += horizontalpoints[textureLocation.x + x] * gaussfilter[x];

	output[dispatchThreadID.xy] = color;

}