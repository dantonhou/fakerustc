# fakerustc
 Fake rustc to trick Cargo into building C++ code
 
# why?
I got bored and had a funny idea in economics class.

# Build and usage
```
g++ fakerustc.cpp -o fakerustc
```
Put the file in path and add 
```toml
[build]
rustc = "fakerustc"
```
to your .cargo/config.toml

use ``cargo init`` to create a new rust project, put your C++ in ``main.rs`` like normal.