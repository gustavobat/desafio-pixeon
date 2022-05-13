# Desafio Pixeon

Este projeto faz parte do processo seletivo para o cargo de Desenvolvedor C++ da **[Pixeon](https://www.pixeon.com/)**.

A avaliação consiste em desenvolver um visualizador de imagens utilizando o framework **[Qt](https://www.qt.io/)**.

O resultado é descrito abaixo.

## Resultado

A interface gráfica conta com quatro elementos principais: uma barra de menu ao topo, uma lista para seleção de arquivos
no canto esquerdo-superior, um painel de controle no canto esquerdo-inferior e uma área de exibição de imagem à direita. 

A animação a seguir mostra um exemplo de uso deste programa:

![Exemplo de uso do programa](assets/example.gif)

### Comportamentos
#### Geral
- O programa é inicializado sem nenhum arquivo carregado. Para carregar uma imagem clique no botão **Open Image File**,
  ou em **File -> Open Image File**.
 - Na barra de menu há um item para exibir as informações sobre este programa, com o título de **About**.
 - O programa é configurado para ler imagens BMP, PNG e JPEG.
- Vários arquivos podem ser carregados, mas apenas um é exibido por vez.
- Ao carregar um novo arquivo, a imagem é atualizada.
- Os nomes dos arquivos abertos são inseridos na lista. Um duplo clique sobre um item da lista altera o arquivo exibido.
- Por razões estéticas, a lista exibe apenas o nome do arquivo, e não seu caminho absoluto. Se um usuário tenta abrir
 um arquivo de mesmo nome, mas de diretórios diferentes, ele é avisado que isto não é possível através de uma caixa
  de mensagem. Este comportamento é mencionado na seção **Futuras Melhorias**.
- O botão **Delete Image File** remove o arquivo selecionado da lista e muda a exibição para o próximo arquivo
  selecionado, se existir. Caso contrário, a tela inicial é exibida.
- Se nenhum arquivo é selecionado, o botão **Delete Image Files** remove todas as imagens. Este comportamento é
  mencionado na seção **Futuras melhorias**.
  
#### Processamento de imagem
  - Os controles de processamentos de imagens só são ativados se há pelo menos uma imagem carregada.
  - Quando o arquivo é exibido, seu tamanho é modificado para que ocupe toda a área de exibição,
  preservando suas proporções.
  - O _zoom_ do arquivo pode ser alterado através dos botões **Zoom In** e **Zoom Out**.
  O botão **Fit to screen** restaura o tamanho da imagem para que ocupe toda a área de exibição.
  - Os ajustes de brilho e contraste são configurados através dos seus respectivos _sliders_.
  - Quando o arquivo a ser exibido é alterado, os controles voltam para o valor padrão.
  

## Notas de implementação

Para a implementação, foram criadas duas classes: `PXMainWindow` e `PXRenderThread`, resumidas abaixo:

- `PXMainWindow`: responsável pela organização dos componentes gráficos e pelo recebimento dos comandos do usuário.

- `PXRenderThread`: responsável pelas operações de processamento da imagem: zoom, alteração de brilho e contraste.

Um objeto `PXRenderThread` faz parte da estrutura de dados da classe `PXMainWindow`. Cada vez que o usuário demanda uma
mudança na imagem, a _render thread_ é acionada e a imagem renderizada é retornada por um conjunto de `signal/slot` que se
conecta no objeto _main window_. Apesar das operações de processamento serem leves, isto evita que a interface gráfica
fique bloqueada a novos comandos enquanto espera uma imagem ser renderizada.



## Futuras melhorias
