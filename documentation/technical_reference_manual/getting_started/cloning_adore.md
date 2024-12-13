# Cloning the ADORe repository
This guild will show you how to properly clone the ADORe repository

> **ℹ️INFO:**
> By default this guide assumes you have ssh keys configured for GitHub your GitHub account.
> For help on configuring your ssh keys visit: https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account


```bash
git clone git@github.com:DLR-TS/adore.git
cd adore
git submodule update --init --recursive
```

> **⚠️ WARNING:** Failing to update and recursively clone the submodules will result in build failures!

> **ℹ️INFO:** If you would rather clone ADORe anonymously over https please review the [Anonymous Cloning 🔗](../system_and_development/anonymous_cloning.md) guide.


