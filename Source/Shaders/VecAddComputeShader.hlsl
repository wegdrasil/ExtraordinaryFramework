struct Data
{
	float2 vec;
};

StructuredBuffer<Data> inputA;
StructuredBuffer<Data> inputB;
RWStructuredBuffer<Data> output;

[numthreads(256, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	output[DTid.x].vec = inputA[DTid.x].vec + inputB[DTid.x].vec;	
}