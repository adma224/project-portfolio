import boto3
from botocore.exceptions import ClientError

# Initialize a boto3 client for S3
s3_client = boto3.client('s3')

# Bucket name - replace 'my-new-bucket' with your desired bucket name
bucket_name = 'user-data91823871329'

try:
    # Create a new S3 bucket
    s3_client.create_bucket(Bucket=bucket_name)
    print(f"Bucket {bucket_name} created successfully.")
except ClientError as e:
    print(f"An error occurred: {e}")
