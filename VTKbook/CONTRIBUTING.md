# Contributing to the VTK Textbook

This page documents at a very high level how to contribute to the VTK Textbook.
Please check our [developer instructions] for a more detailed guide to developing and contributing to the project, and our [VTK Git README] for additional information.

## Preliminary steps

1. You must have installed a LaTeX distribution on your development machine, [TeX Live](http://tug.org/texlive/) is recommended.

2. It is advisable to install a LaTeX editor such as [TeXstudio](https://www.texstudio.org/). It is convenient since it provides syntax-highlighting, an integrated viewer, reference checking and various assistants.

## Setup

Follow the developer instructions in [Setup]

## Workflow

Read and follow the [developer instructions].

In a nutshell:

1. Update your local `master` branch:

    ```bash
    git fetch upstream
    git checkout master
    git merge upstream/master
    git push
    ```

2. Start a new topic branch:

    ```bash
    git fetch origin
    ```

3. For new development, start the topic from `origin/master`:

    ```bash
    git checkout -b my-topic origin/master
    ```

4. Edit files and create commits (repeat as needed):

    ```bash
    edit file1 file2 file3
    git add file1 file2 file3
    git commit
    ```

5. Push commits in your topic branch to your fork in GitLab:

    ```bash
    git push --set-upstream origin my-topic
    ```

6. Visit your fork in GitLab, browse to the "**Merge Requests**" link on the left, and use the "**New Merge Request**" button in the upper right to create a Merge Request.

7. If the merge request is accepted and merged, checkout and update the `master` branch:

    ```bash
    git fetch upstream
    git checkout master
    git merge upstream/master
    git push
    ```

8. Finally delete the local topic branch:

    ```bash
    git branch -d my-topic
    ```

There are several [Mailing Lists] to coordinate development and to provide support.

[VTK Git README]: Documentation/dev/git/README.md
[developer instructions]: Documentation/dev/git/develop.md
[Create an account]: https://gitlab.kitware.com/users/sign_in
[Fork the textbook]: https://gitlab.kitware.com/vtk/textbook/forks/new
[download instructions]: Documentation/dev/git/download.md#clone
[Setup]: Documentation/dev/git/develop.md#Setup
[Create a Topic]: Documentation/dev/git/develop.md#create-a-topic
[Share a Topic]: Documentation/dev/git/develop.md#share-a-topic
[Create a Merge Request]: Documentation/dev/git/develop.md#create-a-merge-request

[Wiki]: http://www.vtk.org/Wiki/VTK
[Doxygen]: http://www.vtk.org/doc/nightly/html
[Mailing Lists]: http://www.vtk.org/VTK/help/mailing.html
