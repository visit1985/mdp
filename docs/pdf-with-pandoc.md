# Convert to PDF using PANDOC

Here is a simple **bashscript** to convert a `mdp` presentation file (`.md`) to `pdf` format using `pandoc`.

**Usage suggestion**:

- Name this script with mdp2pdf
- Put this script somewhere on your PATH
- Now you can execute anywhere, just by typing:
    - `mdp2pdf awesome-presentation.md` # or
    - `mdp2pdf -s awesome-presentation.md`

```bash
#!/usr/bin/env bash

file=''
as_slides=false
usage='
    mdp2pdf usage:

    mdp2pdf [ --as-slides | -s ] FILE

    converts a mdp presentation markdown to a PDF file.

    Options:
        By default, mdp2pdf converts the mdp presentation
        markdown file to a continuos portrait oriented page.
        This setup can be changed with:

        -s
        --as-slides : PDF document is generated as a landscape
            oriented page and "---" are interpreted as
            new pages.
'
err_argc='error: not enough arguments:'
err_file='error: file not found:'

if [[ $# -lt 1 ]]; then
    echo "$err_argc $#"
    echo "$usage"
    exit 1
fi

if [[ $# -gt 0 ]]; then
    [[ "$1" =~ --as-slides|-s ]] && as_slides=true && shift 1
    [[ ! -f "$1" ]] && echo -e "$err_file $file \n$usage" && exit 1
    file="$1"
fi

cleared=$(mktemp --suffix=.md)
trap 'rm -f "$cleared"' EXIT

sed_pattern='s/(^->\s*(.*))/\2/;/^%/d;s/\s*\^\s*//;s/<-\s*$//'

pandoc_args=(
    "--pdf-engine=tectonic"
    "--syntax-highlighting=breezedark"
    "-f" "markdown"
    "-V" "linkcolor:blue"
    "-V" "geometry:a4paper"
    "-V" "geometry:margin=2cm"
    "-V" "mainfont=DejaVu Serif"
    "-V" "monofont=DejaVu Sans Mono"
    "-o" "${file%.md}.pdf"
)

if [[ "$as_slides" == true ]]; then
    sed_pattern+=';s/^---/\\newpage/'
    pandoc_args+=(
        "-V" "documentclass=extarticle"
        "-V" "classoption=landscape"
        "-V" "fontsize=12pt"
    )
fi

sed -E "$sed_pattern" "$file" > "$cleared"
pandoc "$cleared" "${pandoc_args[@]}"
```

Here `pandoc` is using `tectonic` as PDF engine but it can be changed.
Also, this script assumes user are using `---` (three dashes) for slides delimitation.

For this piece of slide (backslashes were add in  ``` to not break this issue formatting):

```md
---

-> # Operadores e expressões
  
  
## Operadores aritiméticos

Em **C** existem os operadores aritiméticos
comuns a qualquer linguagem de programação:
^

\```c
i = i + 3;  // Adição
i = i - 8;  // Subtração
i = i * 9;  // Multiplicação
i = i / 2;  // Divisão
i = i % 5;  // Módulo
\```
^

Bem seus atalho de atribuição:

\```c
i += 3;  // igual a "i = i + 3"
i -= 8;  // igual a "i = i - 8"
i *= 9;  // igual a "i = i * 9"
i /= 2;  // igual a "i = i / 2"
i %= 5;  // igual a "i = i % 5"
\```
^

Em **C**, não existe o **operador de exponenciação**.
Nesse caso, usa-se as funções `pow()` e similares
providas pela biblioteca `main.h`.

---
```

This is what I get using that script with `--as-slides` flag:

<img width="960" height="600" alt="Image" src="https://github.com/user-attachments/assets/8bd77274-0169-40ae-bfb6-9fd1cacdab31" />

**Tested with**:
- `mdp 1.0.18`
- `pandoc 3.10.2`
- `tectonic 0.16.9`

**System**: Debian GNU/Linux Testing (forky)
