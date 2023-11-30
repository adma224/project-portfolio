import boto3

# Initialize a boto3 client for Kinesis with a specified region
kinesis_client = boto3.client('kinesis', region_name='us-east-1')  # Replace 'us-west-2' with your desired region

# Name of the Kinesis stream
stream_name = 'my_kinesis_stream'

# Create the Kinesis stream with 1 shard
try:
    response = kinesis_client.create_stream(
        StreamName=stream_name,
        ShardCount=1
    )
    print("Stream created successfully:", stream_name)
except Exception as e:
    print("Error creating stream:", e)

