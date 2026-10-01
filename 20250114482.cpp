/*
 * =====================================================================================
 * HISTÓRIA DO PROGRAMA: SISTEMA DE CONTROLE DA FARMÁCIA "VIVA-BEM"
 * =====================================================================================
 * A farmácia comunitária "Viva-Bem" está digitalizando seu estoque de medicamentos.
 * 
 * Para modelar o sistema com precisão, foram criadas três entidades centrais:
 * 
 * 1. Bula: Contém a posologia e as contraindicações do medicamento. Trata-se de uma
 *    relação de COMPOSIÇÃO com o remédio: uma bula não existe solta no estoque sem
 *    estar vinculada ao seu remédio. Quando o remédio é destruído/descartado, sua
 *    bula também deixa de existir.
 * 
 * 2. Fabricante: Representa o laboratório farmacêutico (ex: Medley, Eurofarma). Trata-se
 *    de uma relação de AGREGAÇÃO: o laboratório existe independentemente dos remédios.
 *    Vários remédios apontam para o mesmo fabricante, e se um remédio for removido,
 *    o fabricante continua cadastrado no sistema.
 * 
 * 3. Remedio: Classe principal do estoque. Possui um atributo e um método ESTÁTICOS
 *    para rastrear em tempo real a quantidade total de remédios mantidos no estoque.
 * =====================================================================================
 */

#include <iostream>
#include <string>

using namespace std;

// ==========================================
// CLASSE 1: Fabricante (Usada na Agregação)
// ==========================================
class Fabricante {
    
    string nome;
    string cnpj;

public:
    // Construtor
    Fabricante(string nome, string cnpj) {
        // USO DO PONTEIRO THIS
        this->nome = nome;
        this->cnpj = cnpj;
        cout << "[Construtor Fabricante] Laboratório '" << this->nome << "' cadastrado." << endl;
    }

    // Destrutor
    ~Fabricante() {
        cout << "[Destrutor Fabricante] Laboratório '" << this->nome << "' removido do sistema." << endl;
    }

    // Métodos Getters (Encapsulamento)
    string getNome() const {
        return this->nome;
    }

    string getCnpj() const {
        return this->cnpj;
    }
};

// ==========================================
// CLASSE 2: Bula (Usada na Composição)
// ==========================================
class Bula {
    
    string posologia;
    string contraindicacao;

public:
    // Construtor
    Bula(string posologia = "Conforme orientação médica", string contraindicacao = "Sem contraindicações registradas") {
        // USO DO PONTEIRO THIS
        this->posologia = posologia;
        this->contraindicacao = contraindicacao;
        cout << "[Construtor Bula] Bula criada com sucesso." << endl;
    }

    // Destrutor
    ~Bula() {
        cout << "[Destrutor Bula] Bula destruída junto com o remédio." << endl;
    }

    void exibirBula() const {
        cout << "   -> Posologia: " << this->posologia << endl;
        cout << "   -> Contraindicação: " << this->contraindicacao << endl;
    }
};

// ==========================================
// CLASSE 3: Remedio (Classe Central)
// ==========================================
class Remedio {
    
    string nome;
    float preco;

    // COMPOSIÇÃO: Objeto 'Bula' é membro direto da classe Remedio.
    // O ciclo de vida da Bula está estritamente atrelado ao Remedio.
    Bula bula;

    // AGREGAÇÃO: Ponteiro para 'Fabricante'.
    // O Fabricante existe fora do Remedio e não é destruído com ele.
    Fabricante* fabricante;

    // ATRIBUTO STATIC: Compartilhado por todas as instâncias da classe
    static int totalRemediosEstoque;

public:
    // Construtor
    Remedio(string nome, float preco, string posologia, string contraindicacao, Fabricante* fabricante)
        : bula(posologia, contraindicacao) // Inicialização da Composição
    {
        // USO DO PONTEIRO THIS
        this->nome = nome;
        this->preco = preco;
        this->fabricante = fabricante; // Agregação (associação via ponteiro)

        // Incrementa o contador estático
        Remedio::totalRemediosEstoque++;

        cout << "[Construtor Remedio] Medicamento '" << this->nome << "' adicionado ao estoque." << endl;
    }

    // Destrutor
    ~Remedio() {
        cout << "[Destrutor Remedio] Medicamento '" << this->nome << "' removido do estoque." << endl;
        // Decrementa o contador estático
        Remedio::totalRemediosEstoque--;
    }

    // MÉTODO STATIC: Acessa apenas membros estáticos sem precisar de uma instância ativa
    static int getTotalRemediosEstoque() {
        return Remedio::totalRemediosEstoque;
    }

    void exibirFichaTecnica() const {
        cout << "\n==========================================" << endl;
        cout << " Medicamento: " << this->nome << endl;
        cout << " Preço: R$ " << this->preco << endl;

        // Exibindo dados da Agregação (Fabricante)
        if (this->fabricante != nullptr) {
            cout << " Fabricante: " << this->fabricante->getNome() 
                 << " (CNPJ: " << this->fabricante->getCnpj() << ")" << endl;
        } else {
            cout << " Fabricante: Não informado" << endl;
        }

        // Exibindo dados da Composição (Bula)
        cout << " Informações da Bula:" << endl;
        this->bula.exibirBula();
        cout << "==========================================" << endl;
    }
};

// Inicialização obrigatória do atributo estático fora da classe
int Remedio::totalRemediosEstoque = 0;

// ==========================================
// FUNÇÃO PRINCIPAL (MAIN)
// ==========================================
int main() {
    cout << "=== INICIANDO O SISTEMA DA FARMÁCIA VIVA-BEM ===" << endl << endl;

    // 1. Criando 2 objetos da classe Fabricante (Agregação)
    cout << "--- CADASTRANDO FABRICANTES ---" << endl;
    Fabricante fab1("Eurofarma", "61.190.096/0001-92");
    Fabricante fab2("Medley", "50.908.113/0001-08");
    cout << endl;

    // 2. Criando 1 objeto independente da classe Bula
    cout << "--- CRIANDO UMA BULA AVULSA ---" << endl;
    Bula bulaAvulsa("Tomar 1 comprimido ao dia", "Não usar em caso de gravidez");
    cout << endl;

    // 3. Criando 5 objetos da classe Remedio (Relacionando com os Fabricantes)
    cout << "--- CADASTRANDO 5 REMÉDIOS NO ESTOQUE ---" << endl;
    Remedio r1("Paracetamol 750mg", 12.50, "1 comp. de 6 em 6 horas", "Problemas hepáticos", &fab1);
    Remedio r2("Ibuprofeno 600mg", 18.90, "1 comp. de 8 em 8 horas", "Úlceras gástricas", &fab1);
    Remedio r3("Dipirona 1g", 9.90, "1 comp. de 6 em 6 horas", "Alergia a pirazolonas", &fab2);
    Remedio r4("Amoxicilina 500mg", 34.00, "1 cápsula de 8 em 8 horas", "Alergia a penicilina", &fab2);
    Remedio r5("Omeprazol 20mg", 22.15, "1 cápsula em jejum", "Hipersensibilidade", &fab1);
    cout << endl;

    // 4. Chamando o Método Estático
    cout << "--- CONSULTA AO ESTOQUE ---" << endl;
    cout << "Total de remédios atualmente no estoque (via método static): " 
         << Remedio::getTotalRemediosEstoque() << endl;

    // 5. Exibindo informações detalhadas de alguns remédios
    r1.exibirFichaTecnica();
    r3.exibirFichaTecnica();

    cout << "\n=== FINALIZANDO O PROGRAMA E DESTRUINDO OBJETOS ===" << endl;
    return 0;
}