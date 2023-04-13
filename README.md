<h1 align="center">
    <br>
    <img src="https://user-images.githubusercontent.com/97703272/231634217-b0732d3b-5c85-4b3b-bc1d-f616db1e5b24.png" alt="TerraINF" width="200">
    <br>
    TerraINF
    <br>
</h1>

<h4 align="center">Simplified version of Terraria built with <a href="https://en.wikipedia.org/wiki/C_(programming_language)" target="_blank">C</a> & <a href="https://www.raylib.com/" target="_blank">Raylib</a>.</h4>

<p align="center">
    <a href="#license">License</a> •
    <a href="#contact">Contact</a>
</p>

## Avisos Iniciais

O jogo foi desenvolvido no sistema operacional Windows através do VS Code. Foram adicionadas condições para facilitar a compilação desse em qualquer sistema operacional. No entanto, não há garantia que o código compile em qualquer plataforma. Além disso, conforme as implementações se deram no VS Code, não é garantido o bom funcionamento dos arquivos em outras IDEs. 

Para o bom funcionamento do jogo, são necessários minimamente o arquivo executável do jogo (exe), a pasta levels que contém os níveis 1, 2 e 3 e o nível do editor em formato txt e a pasta resources que contém todos efeitos sonoros, músicas, texturas e fundos utilizados e carregados pelo jogo. Essas pastas devem estar localizadas no mesmo diretório do arquivo executável. As pastas “ranking” e custom_levels são criadas a partir da execução do jogo, portanto não são necessárias para rodar pela primeira vez. No entanto, ao serem deletadas depois da primeira vez, os dados de ranking e níveis customizados são reiniciados. 


## Descrição do Jogo

TerraINF consiste em um jogo de mineração de um jogador cujo objetivo é adquirir a maior pontuação possível ao obter certos tipos de minérios. Com uma pontuação específica (que aumenta em 100 % a cada nível), o jogador evolui de fase, aumentando o seu desafio. Minerar ou quebrar blocos de terra aumentam ou diminuem a energia do jogador. O jogo conta com cinco minérios: três desses (prata, ouro e titânio) aumentam a pontuação e a energia do jogador e os demais (césio e urânio) diminuem a energia desse. A locomoção do jogador horizontalmente é livre, já verticalmente depende de escadas, que são disponibilizadas em vinte unidades no início de cada nível. Para fins de aumentar o desafio, o jogador tem 3 vidas no início do jogo. Caso haja alguma queda maior que 3 blocos, o jogador perde 1 vida e caso a sua energia baixe de 20 unidades, também perde 1 vida (e sua energia é regenerada para a quantidade inicial). Essas vidas são mantidas para os próximos níveis. Quando as vidas chegam a zero, o jogador perde o jogo e pode reiniciá-lo a partir do primeiro nível ou sair para o menu inicial. 

O menu do jogo apresenta as opções de: iniciar o jogo; criar fases customizadas; jogar fases customizadas; acessar ranking de melhores pontuações; e fechar o jogo. 

O ranking de pontos mostra os nomes dos 5 melhores jogadores ordenados pela maior pontuação. Caso seja a primeira vez que se abre o jogo, o ranking mostra 5 jogadores com nomes aleatórios e pontuação zerada. 

O editor de níveis é uma tela interativa onde, através do mouse, pode-se desenhar um novo nível e salvá-lo com um nome personalizado. Esse nível poderá ser acessado na tela de “níveis customizados” através do menu inicial. O sistema armazena até 3 níveis customizados, excluindo automaticamente o mais antigo ao criar-se níveis excedendo a capacidade máxima. Ao selecionar algum nível, o jogador é redirecionado para um jogo customizado, onde a pontuação necessária para finalizar o game depende do número de minérios que aumentam a pontuação que foram colocados. Caso atinja-se essa pontuação, uma tela de vitória aparecerá, permitindo reiniciar ou sair da fase personalizada. Porém, ao perder todas as vidas, uma tela de game over, com as mesmas opções da tela de vitória, é exibida. As pontuações das fases customizadas não são contabilizadas no ranking. 


## Como Usar o Programa

Ao iniciar o arquivo executável, o menu inicial do jogo aparecerá com as cinco opções citadas anteriormente na descrição do jogo e ilustrada na captura abaixo. 


### Menu Inicial 

Pode-se navegar pelas opções do menu através do teclado: ou com as teclas W (para cima) e S (para baixo), ou com as setas para cima e para baixo; e com as teclas ENTER para confirmar a opção desejada. Todos os menus do jogo podem ser navegados dessa forma. 

Ao clicar na opção “Iniciar Jogo”, o jogador visualizará uma tela com o número da fase a ser jogada: 


### Splash Screen do Nível 1 

Após essa tela, será exibido o jogo em si. Nele, é possível acompanhar, através da barra superior localizada na tela, as vidas do jogador (marcadas em verde claro), a sua energia (em vermelho), as suas escadas (em azul), a sua pontuação e a pontuação necessária para passar de fase (em rosa) e fase atual (em roxo). As marcações estão demonstradas na figura abaixo. 


### Nível 1 

Abaixo da HUD está o espaço jogável. As demais bordas são intrespassáveis ao jogador, enquanto os espaços mais claros indicam a área acessível sem necessidade de mineração. A movimentação é feita através ou das teclas W, A, S, D (para cima, esquerda, baixo e esquerda respectivamente), ou das setas do teclado. Para avançar verticamente, utiliza-se escadas, posicionadas através da tecla SHIFT. O modo mineração, ativado com a tecla “1”, possibilita avançar aos espaços previamente bloqueados; para realizar a ação de minerar é necessário indicar, através das teclas, a direção desejada e, então, apertar ESPAÇO. Todos os minérios iniciam ocultos e, após minerá-los, o jogo indica o último item adquirido no centro da HUD (marcado em vermelho) e a nova pontuação após a obtenção do mesmo (em laranja), como observar na imagem a seguir: 


### Mineração no Nível 1 

Também observa-se na imagem acima as escadas colocadas no nível (em azul). Como já explicado na descrição do jogo, ao perder todas as vidas, o jogo acaba, gerando uma tela de game over: 


### Tela de Game Over 

Pode-se selecionar as opções, essas sendo: reiniciar o jogo a partir do nível 1 ou sair para o menu inicial dessa tela, com as teclas descritas anteriormente. Abaixo da mensagem de “Fim de Jogo” é disponibilizado o motivo para o encerramento da fase, podendo ser relacionada às vidas esgotadas ou à impossibilidade da fase ser concluída (caso não haja minérios suficientes). Se o jogador obtiver pontuação melhor que algum membro do ranking, pede-se o nome desse para colocar na tela de ranking de melhores pontuações, como exemplificado abaixo. 


### Tela de High Score 

Inspirado nos antigos jogos do Pacman, o nome armazenado é de exatamente 3 caracteres (se digitados minúsculos, serão convertidos para maiúsculos). Ao digitar os 3 caracteres a opção confirmar é destacada, permitindo retornar à tela de game over: 


### Confirmação na Tela de High Score 

O jogo principal não apresenta tela de vitória, visto que ele tem 3 níveis e a última fase não contém minérios suficientes para atingir a meta estabelecida, mantendo um sistema de ranking saudável. 

Retornando ao menu principal, ao selecionar a opção de editor de nível, o usuário é recebido com uma tela interativa onde pode-se navegar com o mouse ou com o teclado. Com o teclado, utiliza-se A e D ou as flechas laterais para selecionar um minério ou salvar a fase. O mouse também funciona e possibilitando clicar nos itens selecionáveis da HUD. Caso queira sair sem salvar, basta pressionar a tecla ESC para voltar ao menu. Os ítens selecionáveis estão marcados em laranja e a opção de salvar só é selecionada ao passar o mouse por cima e clicar com o botão esquerdo ou através das teclas já mencionadas e pressionando ENTER. 


### Editor de Nível

A tela de salvamento de fase é mostrada a seguir solicita ao usuário um nome para a fase. Esse nome aceita diversos tipos de caracteres, mas as letras salvas serão apenas maiúsculas. O tamanho aceitável é de 1 até 20 caracteres. Após digitar um nome válido, a opção de confirmação é destacada e, caso selecionada com a tecla ENTER, redireciona o jogador ao menu inicial. São armazenadas até 3 fases customizadas, descartando-se as mais antigas. 

### Salvamento de Nível Customizado

Com a nova fase salva, pode-se acessá-la através da opção “Níveis Customizados” do menu inicial. Essa tela apresenta cada fase customizada criada com o seu respectivo nome, sua miniatura e a sua data de criação. Quando não há fases criadas, essa tela é vazia, permitinfo o usuário retornar ao menu inicial ou selecionando a opção “Sair” (mesma teclas de seleção do menu inicial) ou através da tecla ESC. Abaixo mostra-se a tela com a fase recém criada de exemplo para este manual: 


### Menu de Níveis Customizados

Selecionando a fase customizada e pressionando a tecla ENTER, o jogador é redirecionado para a fase desejada. O jogo customizado é muito semelhante ao jogo normal, porém não apresenta tela de Splash Screen, o nome do nível na HUD e a pontuação é calculada a partir dos minérios de pontuação positiva colocados na fase. A figura abaixo elucida bem essas diferenças: 


### Jogo Customizado

Esse jogo apresenta game over e todas as outras mecânicas semelhantes às do jogo original. As únicas diferenças são que existem uma tela de vitória, caso o jogador consiga coletar todos os minérios de pontuação positiva da fase, e o sistema de ranking não é aplicável. Para voltar ao menu inicial, basta pressionar a tecla ESC em qualquer momento do jogo customizado. 


### Tela de Vitória

A última opção do menu inicial mostra o ranking de melhores jogadores do jogo principal. Ela só apresenta a opção “Sair” para retornar ao menu principal. Na figura abaixo, aparece a pontuação que foi obtida na demonstração de high score do jogo principal: 


## License

Distributed under the MIT License. See `LICENSE` for more information.


## Contact

Bruno Samuel - [LinkedIn](https://www.linkedin.com/in/brunosag/) - brunosag@outlook.com.br
<br>
Pedro Lima - [LinkedIn](https://www.linkedin.com/in/pedro-lubaszewski/) - pedro.llima@inf.ufrgs.br
<br>
Ana Cláudia Rodrigues - [LinkedIn](https://www.linkedin.com/in/ana-cl%C3%A1udia-rodrigues-1b3524221/) - rodrigues.ac2001@gmail.com
