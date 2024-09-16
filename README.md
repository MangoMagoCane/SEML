# SEML

SEML (Symbolic Expression Markup Language) is a representation for HTML as LISP sexprs 

### Example Code

SEML input:

```
(html 
  (head (title This is SEML!!!))
  (body (h1 We've got a heading)
    (p and a paragraph with a list!
      (ul (li Item 1!)
        (li Item 1!)
        (li Item 2!)
        (li Item 3!)))))
```

HTML output:

```html
<!DOCTYPE html>

<html>
  <head>
    <title>
      This is SEML!!!
    </title>
  </head>
  <body>
    <h1>
      We've got a heading
    </h1>
    <p>
      and a paragraph with a list! 
      <ul>
        <li>
          Item 1!
        </li>
        <li>
          Item 1!
        </li>
        <li>
          Item 2!
        </li>
        <li>
          Item 3!
        </li>
      </ul>
    </p>
  </body>
</html>
```