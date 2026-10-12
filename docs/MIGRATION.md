# Migration Record

Migration date: 2026-10-12

Source: https://gitlab.com/sc18ijf/comp5530m-group-project-team-fyredon-game-engine
Destination: https://github.com/ShawnYangCG/Fyredon

## Preserved History

All nine GitLab branch tips were imported. Two local branch tips were also preserved under `archive/`. The 11 tracked, uncommitted changes in the local DEV-branch2 checkout were saved as one new commit on `codex/continued-development`, based on the original local dev commit `35d728971e969cc8b3d3a9da79f9fca68cc1dd4e`.

The original GitLab dev tip was `58240b436c0c925507aa67eb14884607ee0494dd`; the original main tip was `61d054aaf57a0a02f4fa5c05a9913228a0743c60`. These newer upstream lines remain separate from the local continuation branch.

## Git LFS Conversion

Thirteen historical file versions exceed GitHub's 100 MiB ordinary Git blob limit. They were converted to Git LFS across the imported local branches, without removing their contents. This includes model checkpoints, FBX libraries, and old build artifacts.

`gitlab-commit-map.csv` contains two columns without a header: original commit SHA, migrated commit SHA. Commit messages (apart from terminal newline normalization), author/committer identities and timestamps, and parent relationships are checked against the original history. Affected commit IDs differ because file storage changed.

A complete original-history Git bundle and original branch-tip inventory are retained outside the repository in the migration workspace. The GitLab repository and original D-drive checkout were not modified.

## Remote Configuration

In the migration checkout:

- `origin` points to GitHub and is the default push remote.
- `upstream` points to GitLab for fetches. Its push URL is set to `DISABLED` to prevent accidental pushes.

Remote settings are local Git configuration and are not inherited by new clones. Set them up in a new clone with:

```sh
git remote add upstream https://gitlab.com/sc18ijf/comp5530m-group-project-team-fyredon-game-engine.git
git remote set-url --push upstream DISABLED
git config remote.pushDefault origin
```

Because LFS conversion changed commit IDs, do not merge the raw GitLab history back blindly: it can reintroduce oversized Git blobs and duplicate history. Review future upstream changes and apply selected patches, or perform a coordinated LFS-aware synchronization.

This migration covers Git branches, commits, authorship and file contents. GitLab issues, merge requests, memberships, and service settings are not migrated.
