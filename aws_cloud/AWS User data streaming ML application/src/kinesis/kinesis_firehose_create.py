import boto3
from botocore.exceptions import ClientError

# Initialize the Kinesis Firehose client
firehose_client = boto3.client('firehose')

# Specify the names of your resources
kinesis_stream_name = 'my_kinesis_stream'  # Name of your Kinesis stream
lambda_function_arn = 'data_enrichment'  # ARN of your Lambda function
s3_bucket_name = 'user-data91823871329'  # Name of your S3 bucket
firehose_delivery_stream_name = 'user-producer'  # Name for the new Firehose delivery stream

try:
    # Create a Kinesis Firehose delivery stream
    response = firehose_client.create_delivery_stream(
        DeliveryStreamName=firehose_delivery_stream_name,
        DeliveryStreamType='KinesisStreamAsSource',
        KinesisStreamSourceConfiguration={
            'KinesisStreamARN': f'arn:aws:kinesis:your-region:your-account-id:stream/{kinesis_stream_name}',
            'RoleARN': 'arn:aws:iam::your-account-id:role/your-firehose-role'
        },
        ExtendedS3DestinationConfiguration={
            'RoleARN': 'arn:aws:iam::your-account-id:role/your-s3-role',
            'BucketARN': f'arn:aws:s3:::{s3_bucket_name}',
            'ProcessingConfiguration': {
                'Enabled': True,
                'Processors': [{
                    'Type': 'Lambda',
                    'Parameters': [{
                        'ParameterName': 'LambdaArn',
                        'ParameterValue': lambda_function_arn
                    }]
                }]
            }
        }
    )
    print(f"Firehose delivery stream created: {firehose_delivery_stream_name}")
except ClientError as e:
    print(f"An error occurred: {e}")
