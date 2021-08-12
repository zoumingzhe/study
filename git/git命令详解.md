# git命令详解

# repository
## git init

## git clone

## git log

## git reset

## git remote
## git fetch

## git pull

## git push
### git push -f

# commit
## git add

## git commit



# branch
## git branch

## git checkout


## git merge
### git merge –-squash
```
branch1：1 → 2 → 3 → 4 → 5
branch2：1 → 2 → 6 → 7 → 8
```

在branch2执行：
`git merge branch1 --squash`

```
branch1：1 → 2 → 3 → 4 → 5
branch2：1 → 2 → 6 → 7 → 8 → 9
```

这里的9是将branch1中3、4、5三次提交压缩成一次提交并merge到branch2。

## git rebase

# tag

# cherry-pick

# 参考
1.[Git 教程](https://www.runoob.com/git/git-tutorial.html)
