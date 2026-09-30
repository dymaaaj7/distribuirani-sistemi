using System.ServiceModel;
namespace G2025.Septembar
{
    [ServiceBehavior(InstanceContextMode=InstanceContextMode.PerSession)]
    public class Chat:IChat
    {
        private static Dictionary<string, IChatCallback> korisnici=new Dictionary<string, IChatCallback>();
        private string mojNadimak;
        private IChatCallback callback;
        public Chat()
        {
            callback=OperationContext.Current.GetCallbackChannel<IChatCallback>();
        }

        public void Registracija(string nadimak)
        {
            this.mojNadimak=nadimak;
            if(!korisnici.ContainsKey(nadimak))
                korisnici[nadimak]=callback;
        }

        public void Posalji(string primalac, string sadrzaj)
        {
            if (korisnici.ContainsKey(primalac))
            {
                korisnici[primalac].PrimljenaPoruka(mojNadimak, DateTime.Now, sadrzaj);
            }
        }
    }
}
