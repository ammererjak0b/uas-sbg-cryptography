#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <sodium.h>
#include "dbghelp.h"

/*
 * Sources:
  -[0]: string toLower(): https://stackoverflow.com/questions/313970/how-to-convert-an-instance-of-stdstring-to-lower-case 

   TestCases:
   - Sign: sign hallo 16544641943421030679123768091919808300608612514288022567375693673222897485341765068054608347255021232504193064651134402931841207232635014332117751325844675647906629671281076089035590519196598587658056554125636418006535444345334351492651750717721586147079415909576324431543821506238531368015814787873807642417955801717281399723875756424169601235401597491039048142243745328670427963261550591585566414445351942362445824271537986370747622707904488264947459745401943824064016127724134451090422971570521124029960525998088483765060623851919156042651987689216799982925780869430176836007201152074768832367378341703111292715621 27574403239035051131872946819866347167681020857146704278959489455371495808902941780091013912091702054173655107751890671553068678721058357220196252209741126079844382785468460148392650865327664312763427590209394030010892407242223919154419584529535976911799026515960540719239702510397552280026357979789679404030258615407957036186786232771389009748272012164101678620153121063370761868687354559449487348857517076609945295690442250845502011997113634484669701995411924070046275297826094541720404531785384656530066098620509763736863731979993621950117875452059393485146230621374180318898061869318623963030564089943036087230723
   - Verify: verify hallo 9826314193919104149108053433248973855784374819892819984143204670739543526976780043853415288731530420634560987513117969230223639460245043117954376281586211617078229935083819907305116414773799140165100655684519415382475144899355876263462214993256099349335186564202100778057932018073795653708776359512086458235823141969473900118106303244643961979917712391914541046144256768458881635416253204906114894912949563524257818581988220156912945185150607905377207380052830760307070779596335874966021875181736093992279571117886948399075645520836030281850045779857724835477994028088380667793835794303408873476146768378412061704107 5 27574403239035051131872946819866347167681020857146704278959489455371495808902941780091013912091702054173655107751890671553068678721058357220196252209741126079844382785468460148392650865327664312763427590209394030010892407242223919154419584529535976911799026515960540719239702510397552280026357979789679404030258615407957036186786232771389009748272012164101678620153121063370761868687354559449487348857517076609945295690442250845502011997113634484669701995411924070046275297826094541720404531785384656530066098620509763736863731979993621950117875452059393485146230621374180318898061869318623963030564089943036087230723
    - Verify returns: "Signature valid."
*/

template <size_t N> // N = array size in bytes
void libsodium_to_GMP(const unsigned char (&libsodium_value)[N], mpz_class &GMP_value);
mpz_class hashToGMP(const char * const message, size_t messageLenght);
void signatureToFile(const std::string &fileName, const mpz_class &signature);

int main(const int argc, const char * const argv [])
{
  if (sodium_init() == -1){
    return 1;
  }

  if (argc < 2) 
  {
    std::cerr << "Usage: " << argv[0] << " sign <message> <d> <N>" << std::endl;
    std::cerr << "Usage: " << argv[0] << " verify <message> <signature> <e> <N>" << std::endl;
    return 2;
  }
  
  std::string mode = argv[1];
  std::transform(mode.begin(), mode.end(), mode.begin(),[](unsigned char c){ return std::tolower(c);});  // ugly way but the most okayish way to string.ToLower() [0]: 

  const char * const message = argv[2]; // message: char as message input. Could use string but code was already there. 
  const size_t message_length = strlen(message);
  mpz_class hashValue = hashToGMP(message, message_length);

  // Verify which one of the two modes is selected by user
  if(mode == "sign" && argc == 5){
  
    const mpz_class d(argv[3]); // decimal string -> number
    const mpz_class N(argv[4]);
    mpz_class signature;
    
    mpz_powm(signature.get_mpz_t(), hashValue.get_mpz_t(), d.get_mpz_t(), N.get_mpz_t()); // s = h^d mod N
    
    std::cout << "Signature: " << signature << std::endl; // only the number    
    signatureToFile("signature.txt", signature);    
  }
  else if(mode == "verify" && argc == 6){
    const mpz_class signature(argv[3]);
    const mpz_class e(argv[4]);
    const mpz_class N(argv[5]);

    mpz_class hashRecoverd;
    mpz_powm(hashRecoverd.get_mpz_t(), signature.get_mpz_t(), e.get_mpz_t(), N.get_mpz_t()); // h' = s^e mod N

    // verify if same
    if (hashRecoverd == hashValue)
    {
      std::cout << "Signature valid." << std::endl;
    }
    else
    {
      std::cout << "Signature invalid." << std::endl;
    }

  }
  else{
    std::cerr << "ERROR: No such mode as: " << mode << std::endl;
  }
  
  return 0;
}

template <size_t N> // N = array size in bytes
void libsodium_to_GMP(const unsigned char (&libsodium_value)[N], mpz_class &GMP_value) // from the assignment
{
  GMP_value = 0;
  for (const auto &libsodium_byte : libsodium_value)
  {
    GMP_value *= 256; // shift 1 byte left
    GMP_value += libsodium_byte; // add next byte
  }
}

mpz_class hashToGMP(const char * const message, size_t messageLenght){
  // hash message with sha512 
  unsigned char hash[crypto_hash_sha512_BYTES];
  crypto_hash_sha512(hash, reinterpret_cast<const unsigned char*>(message), messageLenght);

  mpz_class hashValueGMP;
  libsodium_to_GMP(hash, hashValueGMP); 

  return hashValueGMP;
}

void signatureToFile(const std::string &fileName, const mpz_class &signature){
  std::ofstream outputFile("./" + fileName); // workingDir
  outputFile << signature << std::endl; // decimal text
}
