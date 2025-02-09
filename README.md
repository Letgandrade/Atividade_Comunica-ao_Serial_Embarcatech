# Projeto: Controle de LED, Display e Matriz WS2812 com BitDogLab

## Descrição do Projeto

Este projeto utiliza a placa **BitDogLab** para controlar diversos componentes eletrônicos via GPIOs. Os principais elementos utilizados são:

- **Matriz 5x5 de LEDs WS2812**, conectada à GPIO 7.
- **LED RGB**, com os pinos conectados às GPIOs 11, 12 e 13.
- **Botão A**, conectado à GPIO 5.
- **Botão B**, conectado à GPIO 6.
- **Display OLED SSD1306**, conectado via I2C (às GPIOs 14 e 15).

## Funcionalidades Implementadas

### 1. Modificação da Biblioteca `font.h`

- Foram adicionados caracteres minúsculos.

### 2. Entrada de Caracteres via Serial Monitor

- O projeto permite a digitação de caracteres via **Serial Monitor do VS Code**.
- Cada caractere digitado é exibido no display SSD1306.
- Se um número entre **0 e 9** for digitado, a matriz WS2812 exibe um símbolo correspondente ao número.

### 3. Interação com o Botão A

- Ao pressionar o **Botão A**, o LED RGB **Verde** alterna entre ligado e desligado.
- O estado do LED é exibido no display SSD1306.
- Uma mensagem informativa é enviada para o Serial Monitor.

### 4. Interação com o Botão B

- Ao pressionar o **Botão B**, o LED RGB **Azul** alterna entre ligado e desligado.
- O estado do LED é exibido no display SSD1306.
- Uma mensagem informativa é enviada para o Serial Monitor.

## Requisitos do Projeto

### 1. Uso de Interrupções

- As funcionalidades relacionadas aos botões foram implementadas utilizando **rotinas de interrupção (IRQ)**.

### 2. Debouncing

- Foi implementado um **tratamento de bouncing via software** para evitar leituras erradas nos botões.

### 3. Controle de LEDs

- O projeto implementa controle de LEDs comuns e LEDs **WS2812**.

### 4. Utilização do Display OLED SSD1306

- O display 128x64 é utilizado para exibição de caracteres, demonstrando o uso do **protocolo I2C**.

### 5. Envio de Informação pela Serial

- Os eventos do sistema (pressionamento de botões e caracteres digitados) são enviados via **Serial Monitor**.

### 6. Organização do Código

- O código é estruturado e comentado para facilitar a compreensão e manutenção.

## Como Executar o Projeto

### 1. Configurar o Ambiente

- Certifique-se de ter o **VS Code** instalado com a extensão **Pico SDK**.
- Clone este repositório
 
- Acesse a pasta do projeto

### 2. Monitorar a Execução

- Abra o **Serial Monitor** no VS Code para interagir com o projeto.


## Autor

- **Seu Nome**: Letícia Galvão
- **Link Vídeo**: https://drive.google.com/file/d/15HcxfJP7FLPPVGXk7Q4P2JzCo0DW01pW/view?usp=sharing
  



