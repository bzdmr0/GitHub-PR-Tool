from github import Github
import os
import subprocess

# Initialize GitHub client with your personal access token from environment variable
# Set your token with: export GITHUB_TOKEN="your_token_here"
token = os.getenv('GITHUB_TOKEN') or "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"
if not token:
    print("Error: Please set your GitHub token as an environment variable:")
    print("export GITHUB_TOKEN='your_token_here'")
    exit(1)

g = Github(token)

####afasdfasdf

def get_current_git_branch():
    """Get the current Git branch name"""
    try:
        result = subprocess.run(['git', 'branch', '--show-current'], 
                              capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return None

def create_pull_request():
    try:
        # Get repository details from user
        repo_name = "bzdmr0/GitHub-PR-Tool"
        
        # Get the repository
        repo = g.get_repo(repo_name)
        
        # Get pull request details from user
        title = input("Enter pull request title: ").strip()
        if not title:
            title = "New Pull Request"
            
        body = input("Enter pull request description: ").strip()
        if not body:
            body = "Description of changes"
            
        # Get current Git branch automatically
        head_branch = get_current_git_branch()
        if not head_branch:
            print("Could not detect current Git branch")
            head_branch = input("Enter source branch (branch to merge FROM): ").strip()
            
        base_branch = input("Enter target branch (branch to merge TO): ").strip()
        if not base_branch:
            base_branch = "main"
            print(f"Using default target branch: {base_branch}")
        
        # Create a pull request
        pr = repo.create_pull(
            title=title,
            body=body,
            head=head_branch,  # The branch you want to merge FROM
            base=base_branch   # The branch you want to merge TO
        )
        
        print(f"Pull request created successfully!")
        print(f"PR URL: {pr.html_url}")
        print(f"PR Number: {pr.number}")
        
        return pr
        
    except Exception as e:
        print(f"Error creating pull request: {e}")
        return None

if __name__ == "__main__":
    print("GitHub Pull Request Manager")
    print("=" * 40)
    create_pull_request()
