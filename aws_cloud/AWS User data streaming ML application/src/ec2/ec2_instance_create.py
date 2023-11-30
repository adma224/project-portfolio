import boto3
import json
import time
import base64

role_name = 'stream-role'

# User data script for EC2 instance
user_data_script = '''#!/bin/bash
sudo yum update -y
sudo yum install -y python3-pip
sudo pip3 install boto3 requests
cat <<EOF > /home/ec2-user/producer.py
import requests
import json
import boto3
import uuid
import time
import random

region_name = 'us-east-1'
stream_name = 'my-kinesis-stream'

client = boto3.client('kinesis', region_name=region_name)
partition_key = str(uuid.uuid4())
number_of_results = 500

while True:
    r = requests.get(f'https://randomuser.me/api/?exc=login&results={number_of_results}')
    data = r.json()['results']
    random_user_index = int(random.uniform(0, number_of_results))
    random_user = json.dumps(data[random_user_index])
    response = client.put_record(StreamName=stream_name, Data=random_user, PartitionKey=partition_key)
    time.sleep(random.uniform(0, 1))
EOF
python3 /home/ec2-user/producer.py &
'''

# Base64 encode the user data script
encoded_user_data = base64.b64encode(user_data_script.encode()).decode()

# Set up your AWS credentials and region
aws_region = 'us-east-1'
ami_id = 'ami-0080e4c5bc078760e'
instance_type = 't2.micro'
iam_role_name = role_name  # Use the same role name

# Create an EC2 client
ec2 = boto3.client('ec2', region_name=aws_region)

# Launch the EC2 instance with user data
try:
    instance = ec2.run_instances(
        ImageId=ami_id,
        MinCount=1,
        MaxCount=1,
        InstanceType=instance_type,
        IamInstanceProfile={'Name': iam_role_name},
        UserData=encoded_user_data
    )
    instance_id = instance['Instances'][0]['InstanceId']
    print(f"EC2 Instance Created: {instance_id}")

except Exception as e:
    print(f"Error creating EC2 instance: {e}")
