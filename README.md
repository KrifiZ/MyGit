# MyGit

A minimal reimplementation of Git in C++20, written as a learning project to understand how Git works under the hood.

MyGit stores data in a `.mygit` directory using the same object model as Git: zlib-compressed, SHA-1-addressed blob, tree and commit objects. Because the object format matches Git byte for byte, real Git can read the objects MyGit writes (see [Comparison with real Git](#comparison-with-real-git)).

## Commands

| Command | What it does |
|---|---|
| `mygit init` | Creates `.mygit/` with `objects/`, `refs/heads/main` and `HEAD` |
| `mygit hash-object <file>` | Stores a file as a blob and prints its hash |
| `mygit cat-file <hash>` | Decompresses an object and prints its type and content |
| `mygit add <file>...` / `mygit add .` | Stages files into the index (`add .` skips ignored files) |
| `mygit ls-files` | Lists staged files with their blob hashes |
| `mygit write-tree` | Builds tree objects from the index, including nested directories |
| `mygit commit -m "msg"` | Creates a commit object and moves the current branch to it |
| `mygit rev-parse` | Prints the current branch ref and the commit it points to |
| `mygit log` | Walks the history from `HEAD` through each commit's parent |
| `mygit ls-tree <tree-hash>` | Lists all files in a tree, recursing into subdirectories |
| `mygit status` | Shows staged, unstaged and untracked changes |

### `.mygitignore`

A `.mygitignore` file in the repository root supports three kinds of rules:

```
# comment
secret.txt      <- exact path
out/            <- whole directory
*.log           <- file extension
```

Like Git, ignore rules apply only to **untracked** files. A file that is already in the index still shows up in `status` when it changes.

## How it works

- **Objects**: an object is stored as `"<type> <size>\0<content>"`, hashed with SHA-1 (OpenSSL) and compressed with zlib into `.mygit/objects/xx/yyyy...`.
- **Index**: `.mygit/index.txt`, one `<hash> <path>` line per staged file. This is a simplified text format; Git uses a binary `.git/index` with file metadata.
- **Trees**: each entry is `"<mode> <name>\0<20 raw hash bytes>"`. Directories (`40000`) point to subtrees, files (`100644`) to blobs.
- **Commits**: `tree`, optional `parent`, `author`, `committer`, a blank line, then the message.
- **Status** compares three `path -> hash` maps:
  - `HEAD` (read recursively from the last commit's tree) vs index gives "Changes to be committed"
  - index vs working directory gives "Changes not staged for commit"
  - working files missing from the index (and not ignored) are listed as "Untracked files"

## Building

Requirements: Windows, Visual Studio with CMake and vcpkg. Dependencies (`openssl`, `zlib`) are installed by vcpkg from `vcpkg.json`.

```sh
cmake --preset x64-debug
cmake --build out/build/x64-debug
```

The executable is `out/build/x64-debug/MyGit/MyGit.exe`.

## Testing

Every feature was tested by hand in a fresh, throwaway repository. Each test scripted a scenario (create files, `add`, `commit`, edit or delete files), ran MyGit, and checked the output against the expected result from the task description. All checks below pass.

### `log`

| Scenario | Expected | Result |
|---|---|---|
| 3 commits (`first`, `second`, `third`) | listed newest to oldest, stops after the root commit | ✅ |
| Repository with no commits | prints nothing, exits with code 0 | ✅ |

### `status`

Tested with nested directories (`src/main.cpp`, `src/lib/x.h`):

| Scenario | Expected | Result |
|---|---|---|
| No commits yet | all files under "Untracked files" | ✅ |
| Right after a commit | all sections empty | ✅ |
| File edited, not staged | `modified:` under "not staged" | ✅ |
| File deleted from disk | `deleted:` under "not staged" | ✅ |
| New file, not staged | listed under "Untracked files" | ✅ |
| After `add .` | `new file:` / `modified:` under "to be committed" | ✅ |
| After the next commit | all sections empty again | ✅ |
| File two directories deep changed | detected as `src/lib/x.h` | ✅ |

### `.mygitignore`

Rules used: `# secret.txt`, `secret.txt`, `out/`, `*.log`.

| Scenario | Expected | Result |
|---|---|---|
| `secret.txt` | hidden from "Untracked", not added by `add .` | ✅ |
| `out/a.exe`, `out/deep/b.exe` | hidden | ✅ |
| `outside.txt` | **not** hidden by `out/` | ✅ |
| `x.log`, `logs/y.log` | hidden | ✅ |
| `x.log.txt` | **not** hidden by `*.log` | ✅ |
| Only `# secret.txt` in the file | nothing is ignored | ✅ |
| `x.log` added explicitly, then edited | shows `modified: x.log` despite `*.log` | ✅ |

### Comparison with real Git

The same files were added to both a MyGit repository and a real Git repository (Git 2.53, `core.autocrlf=false`), and the results were compared:

| Check | Result |
|---|---|
| Blob hash, `mygit hash-object` vs `git hash-object --no-filters` (file with and without a trailing newline) | identical |
| Staged file hashes, `mygit ls-files` vs `git ls-files -s` | identical |
| Tree hash with nested directories, `mygit write-tree` vs `git write-tree` | identical |
| MyGit commit objects copied into `.git/objects` and read with `git log` | Git shows both commits with the correct messages, author and parent link |
| `git fsck --full` on those objects | passes with no errors |

Commit hashes themselves differ between the two tools, because a commit includes the author and timestamp.

## Known limitations

- Windows only (uses `_setmode` / `<io.h>`).
- The author is hard-coded, and only the current branch ref is updated (no `branch` or `checkout`).
- `add .` does not remove files from the index after they are deleted from disk.
- `ls-tree` expects a tree hash; passing a commit hash makes it loop forever.
