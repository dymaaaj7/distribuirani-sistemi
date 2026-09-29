using System.ServiceModel;

namespace G2025.Septembar
{
    public class ChatCallback : IChatCallback
    {
        public void PrimljenaPoruka(string posiljalac, DateTime vreme, string sadrzaj)
        {
            Console.WriteLine("[" + vreme + "] " + posiljalac + ": " + sadrzaj);
        }
    }

    public class Klijent
    {
        public static void Main(string[] args)
        {
            var factory = new DuplexChannelFactory<IChat>(
                new InstanceContext(new ChatCallback()),
                new WSDualHttpBinding(),
                new EndpointAddress("http://localhost:8000/Chat"));
            IChat proxy = factory.CreateChannel();

            Console.Write("Unesi nadimak: ");
            string nadimak = Console.ReadLine();
            proxy.Registracija(nadimak);

            Console.Write("Kome saljes (ili 'kraj'): ");
            string primalac = Console.ReadLine();
            while (primalac != "kraj")
            {
                Console.Write("Poruka: ");
                string sadrzaj = Console.ReadLine();
                proxy.Posalji(primalac, sadrzaj);

                Console.Write("Kome saljes (ili 'kraj'): ");
                primalac = Console.ReadLine();
            }
        }
    }
}
