# Desafio Pixeon

Este projeto faz parte do processo seletivo para o cargo de Desenvolvedor C++ da **[Pixeon](https://www.pixeon.com/)**.

A avaliação consiste em desenvolver um visualizador de imagens utilizando o framework **[Qt](https://www.qt.io/)**.
O resultado é descrito abaixo.

## Resultado



### Comportamentos

- O programa é inicializado sem nenhum arquivo carregado. Para carregar uma imagem clique no botão **Open Image File**,
  ou em **File -> Open Image File**.
- Vários arquivos podem ser carregados, mas apenas um é exibido por vez.
- Ao carregar um novo arquivo, a imagem é atualizada.
- Os nomes dos arquivos abertos são inseridos na lista. Um duplo clique sobre um item da lista altera o arquivo exibido.
- O botão **Delete Image File** remove o arquivo selecionado da lista e muda a exibição para o próximo arquivo
  selecionado, se existir. Caso contrário, a tela inicial é exibida.
- Se nenhum arquivo é selecionado, o botão **Delete Image Files** remove todas as imagens. Este comportamento é
  mencionado na seção **Futuras melhorias**.

## Notas de implementação

Para a implementação, foram criadas duas classes: `PXMainWindow` e `PXRenderThread`, resumidas abaixo:

- `PXMainWindow`: responsável pela organização dos componentes gráficos e pelo recebimento dos comandos do usuário.

- `PXRenderThread`: responsável pelas operações de processamento da imagem: zoom, alteração de brilho e contraste.

Um objeto `PXRenderThread` faz parte da estrutura de dados da classe `PXMainWindow`. Cada vez que o usuário demanda uma
mudança na imagem, a _render thread_ é acionada e a imagem renderizada é retornada por um conjunto de `signal/slot` que se
conecta no objeto _main window_. Apesar das operações de processamento serem leves, isto evita que a interface gráfica
fique bloqueada a novos comandos enquanto espera uma imagem ser renderizada.



## Futuras melhorias
