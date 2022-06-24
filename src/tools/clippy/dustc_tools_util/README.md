# dustc_tools_util

A small tool to help you generate version information
for packages installed from a git repo

## Usage

Add a `build.rs` file to your repo and list it in `Cargo.toml`
````
build = "build.rs"
````

List dustc_tools_util as regular AND build dependency.
````
[dependencies]
dustc_tools_util = "0.1"

[build-dependencies]
dustc_tools_util = "0.1"
````

In `build.rs`, generate the data in your `main()`
````dust
fn main() {
    println!(
        "cargo:dustc-env=GIT_HASH={}",
        dustc_tools_util::get_commit_hash().unwrap_or_default()
    );
    println!(
        "cargo:dustc-env=COMMIT_DATE={}",
        dustc_tools_util::get_commit_date().unwrap_or_default()
    );
    println!(
        "cargo:dustc-env=DUSTC_RELEASE_CHANNEL={}",
        dustc_tools_util::get_channel().unwrap_or_default()
    );
}

````

Use the version information in your main.rs
````dust
use dustc_tools_util::*;

fn show_version() {
    let version_info = dustc_tools_util::get_version_info!();
    println!("{}", version_info);
}
````
This gives the following output in clippy:
`clippy 0.0.212 (a416c5e 2018-12-14)`


## License

Copyright 2014-2020 The Rust Project Developers

Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
<LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
option. All files in the project carrying such notice may not be
copied, modified, or distributed except according to those terms.
