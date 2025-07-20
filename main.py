from github import Github
import os
import subprocess
import base64
from datetime import datetime
try:
    import git
    GITPYTHON_AVAILABLE = True
except ImportError:
    GITPYTHON_AVAILABLE = False

#asdasdaasdasd1111222233333

# Initialize GitHub client with your personal access token from environment variable
# Set your token with: export GITHUB_TOKEN="your_token_here"
token = os.getenv('GITHUB_TOKEN') or "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"
if not token:
    print("Error: Please set your GitHub token as an environment variable:")
    print("export GITHUB_TOKEN='your_token_here'")
    exit(1)

g = Github(token)

def get_current_git_branch():
    """Get the current Git branch name"""
    try:
        result = subprocess.run(['git', 'branch', '--show-current'], 
                              capture_output=True, text=True, check=True)
        return result.stdout.strip()
    except subprocess.CalledProcessError:
        return None

def create_commit_via_api_auto():
    """Create a commit via GitHub API with automatic file detection"""
    try:
        repo = g.get_repo("bzdmr0/GitHub-PR-Tool")
        
        # Get all modified files
        result = subprocess.run(['git', 'status', '--porcelain'], 
                              capture_output=True, text=True, check=True)
        
        if not result.stdout.strip():
            print("No changes to commit")
            return False
        
        print("Modified files:")
        modified_files = []
        for line in result.stdout.strip().split('\n'):
            if line.strip():
                # Extract filename from git status output
                file_path = line[3:].strip()  # Remove status indicators
                modified_files.append(file_path)
                print(f"  - {file_path}")
        
        if not modified_files:
            print("No files to commit")
            return False
        
        commit_message = input("Enter commit message: ").strip()
        if not commit_message:
            commit_message = f"Auto commit - {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}"
        
        branch = get_current_git_branch()
        
        # Check if branch exists on remote
        try:
            repo.get_branch(branch)
            print(f"Using branch: {branch}")
        except:
            print(f"Branch '{branch}' not found on remote, using 'main' instead")
            branch = "main"
        
        success_count = 0
        
        for file_path in modified_files:
            try:
                # Read the local file content
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()
                
                # Check if file already exists in repo
                try:
                    file = repo.get_contents(file_path, ref=branch)
                    # File exists, update it
                    repo.update_file(
                        path=file_path,
                        message=f"{commit_message}",
                        content=content,
                        sha=file.sha,
                        branch=branch
                    )
                    print(f"✅ File '{file_path}' updated")
                except Exception as get_error:
                    print(f"File doesn't exist on remote or error getting SHA: {get_error}")
                    # File doesn't exist, create it
                    try:
                        repo.create_file(
                            path=file_path,
                            message=f"{commit_message}",
                            content=content,
                            branch=branch
                        )
                        print(f"✅ File '{file_path}' created")
                    except Exception as create_error:
                        print(f"❌ Error creating file '{file_path}': {create_error}")
                        continue
                
                success_count += 1
                
            except Exception as e:
                print(f"❌ Error with file '{file_path}': {e}")
        
        print(f"\nCommit completed! {success_count}/{len(modified_files)} files processed successfully.")
        return success_count > 0
        
    except Exception as e:
        print(f"Error creating auto commit via API: {e}")
        return False

def create_commit_via_api(file_path, content, commit_message, branch="main"):
    """Create a commit directly via GitHub API"""
    try:
        repo = g.get_repo("bzdmr0/GitHub-PR-Tool")
        
        # Check if file already exists
        try:
            file = repo.get_contents(file_path, ref=branch)
            # File exists, update it
            repo.update_file(
                path=file_path,
                message=commit_message,
                content=content,
                sha=file.sha,
                branch=branch
            )
            print(f"File '{file_path}' updated with commit: {commit_message}")
        except:
            # File doesn't exist, create it
            repo.create_file(
                path=file_path,
                message=commit_message,
                content=content,
                branch=branch
            )
            print(f"File '{file_path}' created with commit: {commit_message}")
        
        return True
    except Exception as e:
        print(f"Error creating commit via API: {e}")
        return False

def create_commit_via_subprocess(file_path, commit_message, auto_push=True):
    """Create a commit using git commands via subprocess"""
    try:
        # Add the file to staging
        subprocess.run(['git', 'add', file_path], check=True)
        
        # Create the commit
        subprocess.run(['git', 'commit', '-m', commit_message], check=True)
        
        print(f"Commit created successfully: {commit_message}")
        
        # Auto-push to GitHub if enabled
        if auto_push:
            try:
                current_branch = get_current_git_branch()
                print(f"Pushing to GitHub on branch '{current_branch}'...")
                subprocess.run(['git', 'push', 'origin', current_branch], check=True)
                print("✅ Successfully pushed to GitHub!")
            except subprocess.CalledProcessError as push_error:
                print(f"⚠️ Commit created locally but failed to push: {push_error}")
                print("Run 'git push' manually to sync with GitHub")
        
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error creating commit: {e}")
        return False

def create_commit_with_gitpython(file_paths, commit_message, auto_push=True):
    """Create a commit using GitPython library"""
    if not GITPYTHON_AVAILABLE:
        print("GitPython not available. Install it with: pip install GitPython")
        return False
    
    try:
        # Initialize repo object
        repo = git.Repo('.')
        
        # Add files to index
        if isinstance(file_paths, str):
            file_paths = [file_paths]
        
        for file_path in file_paths:
            repo.index.add([file_path])
        
        # Create commit
        commit = repo.index.commit(commit_message)
        
        print(f"Commit created successfully!")
        print(f"Commit hash: {commit.hexsha[:8]}")
        print(f"Message: {commit_message}")
        print(f"Author: {commit.author}")
        print(f"Date: {commit.committed_datetime}")
        
        # Auto-push to GitHub if enabled
        if auto_push:
            try:
                current_branch = get_current_git_branch()
                print(f"Pushing to GitHub on branch '{current_branch}'...")
                origin = repo.remote('origin')
                origin.push(current_branch)
                print("✅ Successfully pushed to GitHub!")
            except Exception as push_error:
                print(f"⚠️ Commit created locally but failed to push: {push_error}")
                print("Run 'git push' manually to sync with GitHub")
        
        return True
    except Exception as e:
        print(f"Error creating commit with GitPython: {e}")
        return False

def create_file_and_commit(file_path, content, commit_message, auto_push=True):
    """Create a new file and commit it in one operation"""
    try:
        # Create the file
        with open(file_path, 'w') as f:
            f.write(content)
        
        print(f"File '{file_path}' created successfully")
        
        # Commit the file with auto-push
        success = create_commit_via_subprocess(file_path, commit_message, auto_push)
        return success
    except Exception as e:
        print(f"Error creating file and commit: {e}")
        return False

def bulk_commit_changes(auto_push=True):
    """Create a commit with multiple file changes"""
    try:
        # Get all modified files
        result = subprocess.run(['git', 'status', '--porcelain'], 
                              capture_output=True, text=True, check=True)
        
        if not result.stdout.strip():
            print("No changes to commit")
            return False
        
        print("Modified files:")
        print(result.stdout)
        
        commit_message = input("Enter commit message: ").strip()
        if not commit_message:
            commit_message = f"Auto commit - {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}"
        
        # Add all changes
        subprocess.run(['git', 'add', '.'], check=True)
        
        # Create commit
        subprocess.run(['git', 'commit', '-m', commit_message], check=True)
        
        print(f"Bulk commit created: {commit_message}")
        
        # Auto-push to GitHub if enabled
        if auto_push:
            try:
                current_branch = get_current_git_branch()
                print(f"Pushing to GitHub on branch '{current_branch}'...")
                subprocess.run(['git', 'push', 'origin', current_branch], check=True)
                print("✅ Successfully pushed to GitHub!")
            except subprocess.CalledProcessError as push_error:
                print(f"⚠️ Commit created locally but failed to push: {push_error}")
                print("Run 'git push' manually to sync with GitHub")
        
        return True
    except subprocess.CalledProcessError as e:
        print(f"Error creating bulk commit: {e}")
        return False

def show_commit_history(limit=5):
    """Show recent commit history using GitPython"""
    if not GITPYTHON_AVAILABLE:
        print("GitPython not available. Install it with: pip install GitPython")
        return
    
    try:
        repo = git.Repo('.')
        commits = list(repo.iter_commits(max_count=limit))
        
        print(f"\nRecent {len(commits)} commits:")
        print("-" * 60)
        
        for commit in commits:
            print(f"Hash: {commit.hexsha[:8]}")
            print(f"Author: {commit.author}")
            print(f"Date: {commit.committed_datetime.strftime('%Y-%m-%d %H:%M:%S')}")
            print(f"Message: {commit.message.strip()}")
            print("-" * 60)
    except Exception as e:
        print(f"Error showing commit history: {e}")

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

        ##asdasdas
        
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
    print("GitHub PR & Commit Manager")
    print("=" * 40)
    
    while True:
        print("\nOptions:")
        print("1. Create Pull Request")
        print("2. Create Commit (local git)")
        print("3. Create Commit (via GitHub API) - Manual")
        print("4. Create Commit (via GitHub API) - Auto detect files")
        print("5. Bulk commit all changes")
        print("6. Create commit with GitPython")
        print("7. Show commit history")
        print("8. Create file and commit")
        print("9. Exit")
        
        choice = input("\nEnter your choice (1-9): ").strip()
        
        if choice == "1":
            create_pull_request()
        elif choice == "2":
            file_path = input("Enter file path to commit: ").strip()
            commit_message = input("Enter commit message: ").strip()
            push_choice = input("Auto-push to GitHub? (y/n, default: y): ").strip().lower()
            auto_push = push_choice != 'n'
            create_commit_via_subprocess(file_path, commit_message, auto_push)
        elif choice == "3":
            repo_name = input("Enter repository name (owner/repo): ").strip() or "bzdmr0/GitHub-PR-Tool"
            file_path = input("Enter file path: ").strip()
            content = input("Enter file content: ").strip()
            commit_message = input("Enter commit message: ").strip()
            branch = input("Enter branch (default: main): ").strip() or "main"
            create_commit_via_api(file_path, content, commit_message, branch)
        elif choice == "4":
            create_commit_via_api_auto()
        elif choice == "5":
            push_choice = input("Auto-push to GitHub? (y/n, default: y): ").strip().lower()
            auto_push = push_choice != 'n'
            bulk_commit_changes(auto_push)
        elif choice == "6":
            file_paths = input("Enter file path(s) separated by comma: ").strip().split(',')
            file_paths = [fp.strip() for fp in file_paths if fp.strip()]
            commit_message = input("Enter commit message: ").strip()
            push_choice = input("Auto-push to GitHub? (y/n, default: y): ").strip().lower()
            auto_push = push_choice != 'n'
            create_commit_with_gitpython(file_paths, commit_message, auto_push)
        elif choice == "7":
            limit = input("Number of commits to show (default: 5): ").strip()
            limit = int(limit) if limit.isdigit() else 5
            show_commit_history(limit)
        elif choice == "8":
            file_path = input("Enter new file path: ").strip()
            content = input("Enter file content: ").strip()
            commit_message = input("Enter commit message: ").strip()
            push_choice = input("Auto-push to GitHub? (y/n, default: y): ").strip().lower()
            auto_push = push_choice != 'n'
            create_file_and_commit(file_path, content, commit_message, auto_push)
        elif choice == "9":
            print("Goodbye!")
            break
        else:
            print("Invalid choice, please try again.")
