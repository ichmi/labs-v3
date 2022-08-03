# 42Labs 3º Edição: Aplicação de monitoramento de serviços web.


## Desafio
Monitorar serviços web utilizando 3 protocolos: HTTP, PING e DNS. Para cada protocolo, existirão configurações que definem como o monitoramento vai acontecer, assim como o endereço do serviço monitorado. As configurações de monitoramento estarão em um arquivo com nome `monitoring.db`, onde cada linha representa uma espécie de monitoramento com base nas configurações definidas na mesma linha. O programa monitoring irá procurar e analisar esse arquivo, iniciando o processo de monitoramento.

## Como funciona
#### monitoring.db
O arquivo `monitoring.db` define como a sua aplicação vai agir, por exemplo:
```
http google	HTTP	google.com	GET	200	3
```
A linha indica uma requisicao HTPP utilizando o metodo GET a cada 3 segundos, monitornando se sua staus code permanece OK (`200`). Outros exemplos:
```
intra	HTTP	https://intra.42.fr/	POST	302	9
ping	PING	google.com	1
face	DNS	facebook.com	1	8.8.8.8
```
A estrutura é definida por linhas e colunas, onde cada linha terá as configurações separadas por um TAB, que definem as colunas. Para cada monitoramento, as configurações até a 3º coluna serão as mesmas, já a partir da 4º a configuração é específica, baseando-se no protocolo escolhido na 2º coluna.


#### monitoring.db
O programa monitoring le o arquivo de configuração, e a partir deles, os serviços devem ser monitorados da mesma forma que foram configurados.

Informações dos serviços monitorados sao exibidas na saída padrão de uma forma simples e informações mais detalhadas para análises aprofundadas sao armazenadas no arquivo `monitoring.log` que sera gerado durante a execucao do programa. Deve ser possível também adquirir as informações sucintas, as utilizando o argumento `--simplify` no programa. Also, uma notificação de serviço não saudável sera exibida no Discord.
