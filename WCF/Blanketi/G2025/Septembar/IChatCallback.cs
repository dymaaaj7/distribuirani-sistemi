namespace G2025.Septembar
{
    public interface IChatCallback
    {
        [OperationContract(isOneWay=true)]
        void PrimljenaPoruka(string posiljalac, DateTime vreme, string sadrzaj);
    }
}
